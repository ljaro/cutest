#include "inspector.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QMetaMethod>
#include <QMetaObject>
#include <QQuickItem>
#include <QWidget>
#include <private/qjsvalue_p.h>
#include <private/qqmlmetatype_p.h>
#include <private/qqmldata_p.h>
#include <iostream>
Inspector::Inspector()
{

}

QString Inspector::getObjectName(QObject* object)
{
    if(qobject_cast<QQuickItem*>(object))
    {
        QQmlContext *context = QQmlEngine::contextForObject(object);

        if(context && context->engine())
        {
            return context->nameForObject(object);
        }
        else
        {
            return QString("ctx");
        }
    }
    else if(qobject_cast<QWidget*>(object))
    {
        QWidget* w = qobject_cast<QWidget*>(object);
        return w->objectName();
    }

    return QString("none");
}

QString Inspector::getClassName(QObject* object)
{
    auto mo = object->metaObject();
    return mo->className();
}

QString Inspector::getSuperClassName(QObject* object)
{
    auto mo = object->metaObject();
    return mo->superClass()->className();
}

QVariant Inspector::getPropertyValue(QObject *object, QString propName)
{
    auto mo = object->metaObject();

    for(int i=0;i<mo->propertyCount();i++) {
        auto prop = mo->property(i);
        if(prop.isValid()) {
            auto name = QString::fromLatin1(prop.name());
            if(propName == name) {
                return prop.read(object);
            }
        }
    }

    return QVariant();
}

bool Inspector::hasProperty(QObject *object, QString propName)
{
    auto mo = object->metaObject();

    for(int i=0;i<mo->propertyCount();i++) {
        auto prop = mo->property(i);
        if(prop.isValid()) {
            auto name = QString::fromLatin1(prop.name());
            if(propName == name) {
                return true;
            }
        }
    }

    return false;
}

//TODO not every object returns proper type
QString Inspector::getObjectTypeName(QObject* object)
{
    if(object == nullptr) return QString("null object");

    auto qmlType = QQmlMetaType::qmlType(object->metaObject());

    if(qmlType.isValid())
    {
        return qmlType.qmlTypeName();
    }
    else
        return QString("none");
}

QObject* Inspector::getVisualParent(QObject* object)
{
    if(!object)
    {
        return nullptr;
    }

    if(auto itm = qobject_cast<QQuickItem*>(object))
    {
        return itm->parentItem();
    }
    else if(auto itm = qobject_cast<QWidget*>(object))
    {
        return itm->parentWidget();
    }
    else
    {
        return object->parent();
    }
}

