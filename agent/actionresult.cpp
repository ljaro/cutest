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
    QJsonDocument doc;
    QJsonObject resp;

    resp.insert("id", requestId);
    resp.insert("status", QJsonObject::fromVariantHash(status.getStatuses()));

    if (type == ValueType::Simple)
    {
        resp.insert("context", QString::fromStdString(objectValue.getUuid()));

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
        resp.insert("context", QString::fromStdString(objectValue.getUuid()));
        resp.insert("qobject", objectValue.serialize());
        doc.setObject(resp);
        return doc.toJson();
    }
    else if (type == ValueType::ObjectQtList)
    {
        //TODO in some cases i.e get(...) there is no need to send root.context
        //because we return list
        //in that case:
        //no context in response root
        //test should fail if current context doesn't exists
        //so global no context if statement should be removed
        //and test fail condition must be based on response status

        QJsonArray arr;
        for(auto const & testObj : qAsConst(objectValueList))
        {
            arr.append(testObj.serialize());
        }
        resp.insert("qobject", arr);

        doc.setObject(resp);
        return doc.toJson();
    }
    else
    {
        doc.setObject(resp);
        return doc.toJson();
    }
}
