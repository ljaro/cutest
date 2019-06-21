#include "write.h"
#include "spy.h"
#include <QWindow>
#include <QWidget>
#include <QTest>

using namespace TestRobot::Action;

Write::Write()
{

}

bool Write::execSync(TestObject context,
                           ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    if(text.isEmpty())
    {
        callback(context, ActionStatus::create(this).wrongParams());
        return true;
    }

    auto window = qobject_cast<QWindow*>(Spy::instance()->getRootWindow());
    auto qobject = context.getQObject();

    for (int i=0; i < text.length(); i++)
    {
        QString t = text.at(i);
        if(QWidget* widget = qobject_cast<QWidget*>(qobject))
        {
            //TODO timer used becasue QTest::mouseClick need process event after that
            //it has process event inside but still need to go outside scope
            //otherwise it hang application
            QTimer::singleShot(0, [=](){
                QTest::sendKeyEvent(QTest::Click, widget, Qt::Key_A, t, Qt::NoModifier, -1);
            });
        }
        else
        {
            //TODO timer used becasue QTest::mouseClick need process event after that
            //it has process event inside but still need to go outside scope
            //otherwise it hang application
            QTimer::singleShot(0, [=](){
                QTest::sendKeyEvent(QTest::Click, window, Qt::Key_A, t, Qt::NoModifier, -1);
            });

        }
    }

    callback(context, ActionStatus::create(this).ok());
    return true;
}

AsyncResult Write::execAsync(TestObject context,
                            ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void Write::applyParams(QJsonObject params)
{
    text = params.value("text").toString(QString());
}
