#include "click.h"

#include <QSharedPointer>
#include <QThread>
#include <QCoreApplication>
#include <QTest>
#include <QObject>
#include <QPoint>
#include <QQuickItem>
#include <QWidget>

#include "testobject.h"
#include "spy.h"

using namespace TestRobot::Action;
using namespace std;

Click::Click()
{

}

bool Click::execSync(TestObject context,
                           ActionCallback callback)
{
    if(!context.isValid())
    {
        //TODO need to callback? what context
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    QPoint oPoint;
    int holdTime = 0;
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
            if(holdTime == 0)
            {
                //TODO timer used becasue QTest::mouseClick need process event after that
                //it has process event inside but still need to go outside scope
                //otherwise it hang application
                QTimer::singleShot(0, [=](){
                    QTest::mouseClick(wnd, Qt::LeftButton, Qt::NoModifier, oPoint );
                });
            }
            else if (/*press*/ false)
            {
                //TODO timer used becasue QTest::mouseClick need process event after that
                //it has process event inside but still need to go outside scope
                //otherwise it hang application
                QTimer::singleShot(0, [=](){
                    QTest::mousePress(wnd, Qt::LeftButton, Qt::NoModifier, oPoint );
                });
            }
            else
            {
                //TODO timer used becasue QTest::mouseClick need process event after that
                //it has process event inside but still need to go outside scope
                //otherwise it hang application
                QTimer::singleShot(0, [=](){
                    QTest::mouseRelease(wnd, Qt::LeftButton, Qt::NoModifier, oPoint );
                });
            }
        }
    }

    callback(context, ActionStatus::create(this).ok());
    return true;
}

AsyncResult Click::execAsync(TestObject context,
                            ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void Click::applyParams(QJsonObject params)
{
    this->params = make_shared<ParamProps>();
    if(!params.isEmpty())
    {
        auto delay = params.value("delay").toInt();
        this->params = make_shared<ParamProps>();
        this->params->props.insert(make_pair("delay", QVariant(delay)));
    }
    else
    {
        qWarning() << "Dispatcher::getQueryParams - cannot find json object 'params'";
    }
}
