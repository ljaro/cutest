#ifndef TESTOBJECT_H
#define TESTOBJECT_H
#include <QObject>
#include <QVariantHash>
#include <QJsonObject>
#include <string>
#include <functional>
#include "context.h"

class TestObject;

bool operator==(const TestObject& lhs, const TestObject& rhs);

class TestObject
{
public:
    TestObject();
    explicit TestObject(QString ctxStr);
    explicit TestObject(QObject* qobject);
    ContextStr getUuid() const {return uuid;}
    QObject* getQObject() const {return qobject;}
    bool isValid(){return qobject != nullptr;}

    friend bool operator==(const TestObject& lhs, const TestObject& rhs);
public:
    QJsonObject serialize() const;
    void updateProperty(QString name, QVariant value);
private:
    ContextStr uuid;
    QObject* qobject;
    QVariantHash properties;
    QString typeFromObject(QObject *obj) const;
};

struct TestObjectHash
{
    std::size_t operator()(TestObject const& s) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(s.getUuid());
        std::size_t h2 = std::hash<unsigned int>{}(qHash(s.getQObject()));
        return h1 ^ (h2 << 1);
    }
};

#endif // TESTOBJECT_H
