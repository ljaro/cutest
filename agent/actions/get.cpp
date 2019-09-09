#include "get.h"

#include <QObject>
#include <QQuickItem>
#include <QQmlProperty>
#include <QQuickItem>
#include <QQmlEngine>
#include <QMetaType>
#include <QMetaProperty>
#include <QAbstractItemModel>
#include <QWidget>

#include "inspector.h"
using namespace TestRobot::Action;

Get::Get()
{

}

bool Get::execSync(TestObject context, ActionCallback callback)
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

    //if(QQuickItem* qobject = qobject_cast<QQuickItem*>(context.getQObject()))
    if(QQmlContext* qmlCtx = QQmlEngine::contextForObject(context.getQObject()))
    {

        //if(QQmlContext* qmlCtx = QQmlEngine::contextForObject(qobject))
        if(1)
        {
            QQmlProperty prop(context.getQObject(), property_name, qmlCtx);

            if(prop.type() == QQmlProperty::Type::Property)
            {
                switch (prop.propertyTypeCategory())
                {
                case QQmlProperty::InvalidCategory:
                    callback(context, ActionStatus::create(this).unsupportedType("in invalidCategory"));
                    return true;
                case QQmlProperty::List:
                    handlePropertyList(prop, callback, context);
                    return true;
                case QQmlProperty::Object:
                    handleObject(prop, callback, context);
                    return true;
                case QQmlProperty::Normal:
                    handleNormal(prop, callback, context);
                    return true;
                }
            }
            else if (prop.type() == QQmlProperty::Type::SignalProperty)
            {
                callback(context, ActionStatus::create(this).unsupportedType(QString("property '%1' not found").arg(property_name)));
            }
            else if (prop.type() == QQmlProperty::Type::Invalid)
            {
                callback(context, ActionStatus::create(this).unsupportedType(QString("property '%1' not found").arg(property_name)));
            }
        }
        else
        {
            callback(context, ActionStatus::create(this).contextMustBeQQuickItem());
        }
    }
    else if(QWidget* qobject = qobject_cast<QWidget*>(context.getQObject()))
    {
        //TOOD implement for Widgets
        callback(context, ActionStatus::create(this).contextMustBeQQuickItem());
        return true;
    }
    else if(QObject* qobject = qobject_cast<QObject*>(context.getQObject()))
    {
        auto value = Inspector::getPropertyValue(qobject, property_name);

        if(value.isValid())
        {
            context.updateProperty(property_name, value);
            callback(context, ActionStatus::create(this).ok());
        }
        else
        {
            context.updateProperty(property_name, QVariant());
            callback(context, ActionStatus::create(this).ok());
        }

        return true;
    }
    else
    {
        //TOOD implement for Widgets
        callback(context, ActionStatus::create(this).contextMustBeQQuickItem());
        return true;
    }

    callback(context, ActionStatus::create(this).unsupportedType(""));

    return true;
}

AsyncResult Get::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void Get::applyParams(QJsonObject params)
{
    property_name = params.value("property_name").toString();
}

void Get::handlePropertyList(QQmlProperty prop, ActionCallback callback, TestObject context)
{
    ActionResult result;
    result.type = ActionResult::ValueType::ObjectQtList;

    auto refList = prop.read().value<QQmlListReference>();

    QList<TestObject> lst;
    for(int i = 0; i< refList.count(); ++i)
    {
        auto const& obj = refList.at(i);
        TestObject testObject(obj);
        lst.append(testObject);
    }

    result.objectValueList = lst;

    callback(result, ActionStatus::create(this).ok());
}

void Get::handleNormal(QQmlProperty prop, ActionCallback callback, TestObject context)
{
    QVariant s;
    auto qo = context.getQObject();

    if(prop.propertyType() == QMetaType::QVariant)
    {
        QVariant v = prop.read();
        if(v.type() ==  QVariant::UserType)
        {
            auto jsValue = v.value<QJSValue>();
            if(jsValue.isArray())
            {
                context.updateProperty(property_name, v.toList());
                callback(context, ActionStatus::create(this).ok());
            }
            else
            {
                context.updateProperty(property_name, v.toString());
                callback(context, ActionStatus::create(this).ok());
            }
        }
        else if (v.userType() == QMetaType::Type::QObjectStar)
        {
            auto result = ActionResult(TestObject(v.value<QObject*>()));
            if(v.canConvert<QAbstractItemModel*>())
            {
                callback(result, ActionStatus::create(this).ok());
            }
            else if(v.canConvert<QStringList>())
            {
                callback(result, ActionStatus::create(this).ok());
            }
            else
            {
                callback(result, ActionStatus::create(this).ok());
            }
        }
        else
        {
            callback(context, ActionStatus::create(this).unsupportedType(""));
        }
    }
    else
    {
        auto value = prop.read();
        context.updateProperty(property_name, value);
        callback(context, ActionStatus::create(this).ok());
        //TODO should we really return whole context or just send property
    }


}

void Get::handleObject(QQmlProperty prop, ActionCallback callback, TestObject context)
{
    int type = prop.propertyType();
    QVariant value = prop.read();
    bool isQObject = value.canConvert<QObject*>();
    if(value.canConvert<QObject*>())
    {
        QObject* qobj = qvariant_cast<QObject*>(prop.read());
        auto qquick = qobject_cast<QQuickItem*>(qobj);
        callback(TestObject(qquick), ActionStatus::create(this).ok());
    }
    else
    {
        QString info = QString("%1(%2)").arg(prop.propertyTypeCategory()).arg(prop.propertyTypeName());
        callback(TestObject(), ActionStatus::create(this).unsupportedType(info));
    }
}
