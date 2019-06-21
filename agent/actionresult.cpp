#include "actionresult.h"
#include <QAbstractItemModel>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ActionResult::ActionResult()
{

}

ActionResult::ActionResult(TestObject &testObject)
    : objectValue(testObject),
      type(ValueType::ObjectQt)
{

}

//TODO still copy - TestObject move ctor
ActionResult::ActionResult(TestObject &&testObject)
    : objectValue(testObject),
      type(ValueType::ObjectQt)
{

}

QByteArray ActionResult::createResponse(QString requestId, Status status)
{
    if (type == ValueType::Simple)
    {
        QJsonDocument doc;
        QJsonObject resp;
        resp.insert("id", requestId);
        resp.insert("context", QString::fromStdString(objectValue.getUuid()));
        resp.insert("status", QJsonObject::fromVariantHash(status.getStatuses()));


        if(simpleValue.type() == QVariant::Type::List)
            resp.insert("simple", simpleValue.toJsonArray());
        else
        if(simpleValue.type() == QVariant::Type::Map)
            resp.insert("simple", simpleValue.toJsonObject());
        else
            resp.insert("simple", simpleValue.toJsonValue());


        doc.setObject(resp);
        return doc.toJson();
    }
    else if (type == ValueType::ObjectQt)
    {
        QJsonDocument doc;
        QJsonObject resp;
        resp.insert("id", requestId);
        resp.insert("context", QString::fromStdString(objectValue.getUuid()));
        resp.insert("status", QJsonObject::fromVariantHash(status.getStatuses()));
        resp.insert("qobject", objectValue.serialize());
        doc.setObject(resp);
        return doc.toJson();
    }
    else
    {
        QJsonDocument doc;
        QJsonObject resp;
        resp.insert("id", requestId);
        resp.insert("status", QJsonObject::fromVariantHash(status.getStatuses()));
        doc.setObject(resp);
        return doc.toJson();
    }



}

