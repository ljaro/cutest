#include "testobject.h"
#include <QUuid>
#include <QAbstractItemModel>

TestObject::TestObject() :
    uuid(""),
    qobject(nullptr)
{

}

TestObject::TestObject(QString ctxStr) :
    uuid(ctxStr.toStdString()),
    qobject(nullptr)
{

}

//TODO check duplicates different TestObject with same qobject and different uuid
TestObject::TestObject(QObject *qobject) :
    uuid(QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString()),
    qobject(qobject)
{

}

QJsonObject TestObject::serialize()
{
    QJsonObject result;

    result.insert("context", QString::fromStdString(getUuid()));
    result.insert("properties", QJsonObject::fromVariantHash(properties));
    result.insert("type", typeFromObject(qobject));

    return result;
}

void TestObject::updateProperty(QString name, QVariant value)
{
    properties.insert(name, value);
}

bool operator==(const TestObject &lhs, const TestObject &rhs) {
    return lhs.uuid == rhs.uuid;
}


QString TestObject::typeFromObject(QObject *obj)
{
    if(qobject_cast<QAbstractItemModel*>(obj))
    {
        return "model";
    }

    return "qobject";
}
