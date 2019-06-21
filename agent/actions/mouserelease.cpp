#include "mouserelease.h"

#include <QSharedPointer>
#include <QThread>
#include <QCoreApplication>
#include <QTest>
#include <QObject>
#include <QPoint>
#include <QQuickItem>
#include <QWidget>

#include "testobject.h"
#include "param.h"
#include "spy.h"

using namespace TestRobot::Action;

MouseRelease::MouseRelease()
{

}

bool MouseRelease::execSync(TestObject context,
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
        if(wnd)
        {
            QTest::mouseRelease(wnd, Qt::LeftButton, Qt::NoModifier,{100,100});
        }
    }

    callback(context, ActionStatus::create(this).ok());
    return true;
}

AsyncResult MouseRelease::execAsync(TestObject context,
                                   ActionCallback callback)
{

}

void MouseRelease::applyParams(QJsonObject params)
{

}
