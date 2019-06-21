#include "spy.h"
#include <QCoreApplication>
#include <QApplication>
#include <QThread>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <iostream>
#include "inspector.h"
#include "overlaypainter.h"
QAtomicPointer<Spy> Spy::m_instance = QAtomicPointer<Spy>(nullptr);
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, spylock, (QMutex::Recursive))

Spy::Spy(QObject *parent) : QObject(parent), m_queueTimer(new QTimer(this))
{
    m_queueTimer->setSingleShot(true);
    m_queueTimer->setInterval(0);
    connect(m_queueTimer, &QTimer::timeout, this, &Spy::processQueue);
}

void Spy::create()
{
    Q_ASSERT(qApp->thread() == QThread::currentThread());

    Spy* spy = new Spy();
    QMutexLocker lock(spylock());
    m_instance = QAtomicPointer<Spy>(spy);

    Q_ASSERT(qGuiApp != nullptr);
    if(qGuiApp)
    {
        qDebug() << "*******************************";
    }
    qGuiApp->setApplicationDisplayName("***[Application under test]***");
    spy->rootWindow = qGuiApp;

    //TODO hack
    QTimer::singleShot(2000, [spy](){

        foreach(auto const& wnd , qGuiApp->allWindows())
        {
            if (QQuickWindow *window = qobject_cast<QQuickWindow *>(wnd)) {
                spy->rootWindow = window;
            }
        }

        Q_ASSERT(spy->rootWindow != nullptr);

        OverlayPainter::instance().setWindow(qobject_cast<QQuickWindow*>(spy->rootWindow));
    });

    spy->cache.rebuild(qGuiApp);
}

Spy *Spy::instance()
{
    return m_instance.load();
}

QMutex *Spy::objectLock()
{
    return spylock();
}

void Spy::onObjectCreated(QObject *obj)
{
    QMutexLocker locker(spylock());

    if(filter(obj))
    {
        return;
    }

    if(cache.hasObject(obj))
    {
        return;
    }

    if (obj->parent() && !cache.hasObject(obj->parent()))
    {
        onObjectCreated(obj->parent());
    }

    if(!cache.hasObject(obj) && !newObjectsQueue.contains(obj))
    {
        newObjectsQueue.push_back(obj);
        notifyQueue();
    }
}

void Spy::onObjectDeleted(QObject *obj)
{
    QMutexLocker locker(spylock());

    // obj is invalid here, maybe better solution would be change delete hook to connection?
    // one cons - app can disconnect all connections
    // we can cache data for objects
    if(cache.remove(obj))
    {
        //std::cout << "delete " << static_cast<void*>(obj) << " " << Inspector::getObjectName(obj).toStdString() << std::endl;

        for (int i = 0; i < newObjectsQueue.size(); ++i) {
            if (newObjectsQueue.at(i) == obj) {
                newObjectsQueue.remove(i);
                break;
            }
        }

        emit objectDeleted(obj);
    }
}

void Spy::windowAfterRendering()
{
//    QMutexLocker lock(spylock());

//    QQuickWindow* wnd = qobject_cast<QQuickWindow*>(instance()->getRootWindow());
//    Q_ASSERT(QOpenGLContext::currentContext() == wnd->openglContext());

//    QOpenGLPaintDevice device(wnd->size() * wnd->devicePixelRatio());
//    device.setDevicePixelRatio(wnd->devicePixelRatio());
//    QPainter p(&device);

//    p.save();
//    p.setPen(QColor(255,10,10,55));
//    QBrush b;
//    b.setStyle(Qt::BrushStyle::Dense3Pattern);
//    b.setColor(QColor(155,255,110,130));
//    p.fillRect(spiedRect, b);

//    QPen pen;
//    pen.setColor(QColor(255,0,0,255));
//    pen.setWidth(10);
//    p.setPen(pen);
//    p.drawPoint(clickedPoint);

//    p.restore();
}

bool Spy::filter(QObject *obj) const
{
    return obj == this;// || obj == rootWindow;
}

void Spy::notifyQueue()
{
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
}

void Spy::processQueue()
{
    QMutexLocker lock(spylock());

    Q_ASSERT(QThread::currentThread() == thread());

    for(const auto &obj : qAsConst(newObjectsQueue))
    {
        if(cache.insert(obj))
        {
            //std::cout << "create " << static_cast<void*>(obj) << " " << Inspector::getObjectName(obj).toStdString() << std::endl;
            //TODO move on timer?
            emit objectCreated(obj);
        }
    }

    newObjectsQueue.clear();
}
