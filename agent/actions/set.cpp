#include "set.h"
#include <QQmlProperty>
#include <QQuickItem>

using namespace TestRobot::Action;

Set::Set()
{

}

bool Set::execSync(TestObject context, ActionCallback callback)
{
    if(property_name.isEmpty())
    {
        callback(context, ActionStatus::create(this).wrongParams());
        return true;
    }

    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }


    qDebug() << "CHECK QOBJECT";
    if(QQuickItem* qobject = qobject_cast<QQuickItem*>(context.getQObject()))
    {
        QQmlProperty prop(qobject, property_name);
        prop.write(property_value);
        callback(context, ActionStatus::create(this).ok());
    }
    else
    {
        //TOOD implement for Widgets
        callback(context, ActionStatus::create(this).contextMustBeQQuickItem());
        return true;
    }

    return true;
}

AsyncResult Set::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void Set::applyParams(QJsonObject params)
{
    property_name = params.value("property_name").toString();
    property_value = params.value("property_value").toVariant();
}
