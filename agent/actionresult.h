#ifndef ACTIONRESULT_H
#define ACTIONRESULT_H

#include "testobject.h"
#include "testactionstatus.h"

class ActionResult
{
public:
    enum class ValueType {
        None,
        Simple,
        ObjectQt
    };

    ActionResult();
    ActionResult(TestObject& testObject);
    ActionResult(TestObject&& testObject);

    QByteArray createResponse(QString requestId, Status status);

    ValueType type = ValueType::None;
    TestObject objectValue;
    QVariant simpleValue;
};

#endif // ACTIONRESULT_H
