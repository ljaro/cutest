#ifndef ACTIONRESULT_H
#define ACTIONRESULT_H

#include "testobject.h"
#include "testactionstatus.h"

class ActionResult
{
public:
    enum class ValueType {
        None,
        Simple, // simple can be array but not an array of QQuick/QObjects beacuse they need context
        ObjectQt,
        ObjectQtList
    };

    ActionResult();
    ActionResult(TestObject& testObject);
    ActionResult(TestObject&& testObject);

    QByteArray createResponse(QString requestId, Status status);

    ValueType type = ValueType::None;
    TestObject objectValue;
    QList<TestObject> objectValueList;
    QVariant simpleValue;
};

#endif // ACTIONRESULT_H
