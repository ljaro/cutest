#include "mousepress.h"

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

MousePress::MousePress()
{

}

bool MousePress::execSync(TestObject context,
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
    QObject* wndObj = Spy::instance()->getRootWindow();



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



    if(isWidget)
    {
        QTest::mouseClick(qobject_cast<QWidget*>(qobject), Qt::LeftButton, Qt::NoModifier, oPoint,0 );
    }
    else
    {
        auto wnd = qobject_cast<QWindow*>(wndObj);
        if(wnd)
        {
            if(actionType == ActionType::Press)
            {
                QTest::mousePress(wnd, Qt::LeftButton, Qt::NoModifier, oPoint);
            }
            else if (actionType == ActionType::Move)
            {
                Qt::MouseButtons qtestMouseButtons;
                qtestMouseButtons.setFlag(Qt::LeftButton, true);

                QQuickItem* itm = qobject_cast<QQuickItem*>(qobject);
                auto targetPos = QPoint(200, 40);
                auto tPoint = itm->mapToScene(targetPos);
                lastMove = tPoint.toPoint();

                qDebug() << lastMove;

                QTest::mouseMove(wnd, lastMove);


            }
            else if (actionType == ActionType::Release)
            {
                QTest::mouseRelease(wnd, Qt::LeftButton, Qt::NoModifier);
            }
        }
    }

    callback(context, ActionStatus::create(this).ok());
    return true;
}

AsyncResult MousePress::execAsync(TestObject context,
                                 ActionCallback callback)
{

}

void MousePress::applyParams(QJsonObject params)
{
    x = params.value("x");
    y = params.value("y");

    auto type = params.value("type").toString();
  
    actionType = ActionType::None;
    if(type == "mouse_press") actionType = ActionType::Press;
    if(type == "mouse_release") actionType = ActionType::Release;
    if(type == "mouse_move") actionType = ActionType::Move;

}
