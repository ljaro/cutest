#include "mousemove.h"

#include <QSharedPointer>
#include <QThread>
#include <QCoreApplication>
#include <QTest>
#include <Qt>
#include <QObject>
#include <QPoint>
#include <QQuickItem>
#include <QWidget>
#include <QTouchDevice>
#include "testobject.h"
#include "param.h"
#include "spy.h"

using namespace TestRobot::Action;

MouseMove::MouseMove()
{

}

bool MouseMove::execSync(TestObject context,
                               ActionCallback callback)
{
    if(!context.isValid())
    {
        //TODO need to callback? what context
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    QPoint oPoint;
    bool isWidget = false;
    auto qobject = context.getQObject();

    if(QQuickItem* itm = qobject_cast<QQuickItem*>(qobject))
    {
        auto oPointF = itm->mapToScene( QPoint( 0, 0 ) );
        oPoint = oPointF.toPoint();
        oPoint.rx() += itm->boundingRect().width() / 2;
        oPoint.ry() += itm->boundingRect().height() / 2;
    }
    else if(QWidget* itm = qobject_cast<QWidget*>(qobject))
    {
        isWidget = true;
        auto oPoint = QPoint( 0, 0 );
        oPoint.rx() += itm->rect().width() / 2;
        oPoint.ry() += itm->rect().height() / 2;
    }

    QObject* wndObj = Spy::instance()->getRootWindow();

    if(isWidget)
    {
        QTest::mouseClick(qobject_cast<QWidget*>(qobject), Qt::LeftButton, Qt::NoModifier, oPoint,0 );
    }
    else
    {
        auto wnd = qobject_cast<QWindow*>(wndObj);

        // qApp->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true);
        if(wnd)
        {
            Qt::MouseButtons qtestMouseButtons;
            qtestMouseButtons.setFlag(Qt::LeftButton, true);

            qDebug() << x << ", " << y;

            QQuickItem* itm = qobject_cast<QQuickItem*>(qobject);
            auto targetPos = itm->position() + QPoint(5, 5);


            auto tPoint = itm->mapToScene(targetPos);
            qDebug() << tPoint;
            qDebug() << tPoint.toPoint();
            //QPointF global = wnd->mapToGlobal(tPoint);

         //   QCursor::setPos({500,500});

            QTest::mouseMove(wnd, tPoint.toPoint());
         //   QTest::mouseEvent(QTest::MouseMove, wnd, Qt::LeftButton, Qt::KeyboardModifiers(), tPoint.toPoint());
//            qt_handleMouseEvent(wnd, tPoint, global, qtestMouseButtons,
//                                Qt::LeftButton, QEvent::MouseMove, Qt::NoModifier, 200);


//            QTimer::singleShot(3000, [=](){
//                QTouchDevice *dev = QTest::createTouchDevice();

//                QTest::touchEvent(wnd, dev)
//                    .press(0, QPoint(110, 110), wnd);
//                QTest::touchEvent(wnd, dev)
//                    .move(0, QPoint(55, 55), wnd);
//                QTest::touchEvent(wnd, dev)
//                    .release(0, QPoint(55, 55), wnd);
//            });


//            QMouseEvent eve( (QEvent::MouseMove), QPoint(100,100),
//                    Qt::NoButton,
//                    Qt::NoButton,
//                    Qt::NoModifier   );

//            qApp->sendEvent(wndObj , &eve);
        }
    }

    callback(context, ActionStatus::create(this).ok());
    return true;
}

AsyncResult MouseMove::execAsync(TestObject context,
                                ActionCallback callback)
{

}

void MouseMove::applyParams(QJsonObject params)
{
    x = params.value("x").toInt();
    y = params.value("y").toInt();
}
