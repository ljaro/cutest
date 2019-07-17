#include "testactionstatus.h"
#include "testaction.h"

namespace  {
    QString cannotExecuteStr() { return "cannot execute";};
}

ActionStatus ActionStatus::create(Response *parentAction)
{
    return ActionStatus(parentAction);
}

ActionStatus::ActionStatus(Response *parentAction) :
    parentAction(parentAction)
{

}

Status Status::ok()
{
    values.insert("status", "ok");
    return *this;
}

Status Status::wrongParams()
{
    values.insert("status", cannotExecuteStr());
    values.insert("details", "wrong parameters");
    return *this;
}

Status Status::actionNotFound()
{
    values.insert("status", cannotExecuteStr());
    values.insert("details", "action not found");
    return *this;
}

Status Status::contextNotFound()
{
    values.insert("status", cannotExecuteStr());
    values.insert("details", "context not found");
    return *this;
}

Status Status::objectNotFound()
{
    values.insert("status", "failed");
    values.insert("details", "object not found");
    return *this;
}

Status Status::ancestorNotFound()
{
    values.insert("status", "failed");
    values.insert("details", "ancestor not found");
    return *this;
}

Status Status::globalTimeout()
{
    values.insert("status", "failed");
    values.insert("details", "global timeout");
    return *this;
}

Status Status::moreObjectsFound()
{
    values.insert("status", "failed");
    values.insert("details", "more than one object found");
    return *this;
}

Status Status::contextMustBeQQuickItem()
{
    values.insert("status", "failed");
    values.insert("details", "context must be of type QQuickItem");
    return *this;
}

Status Status::unsupportedType(const QString& typeInfo)
{
    values.insert("status", "failed");
    values.insert("details", "Unsupported type: " + typeInfo);
    return *this;
}

Status Status::notImplemented(QString info)
{
    values.insert("status", "failed");
    values.insert("details", "Not implemented: " + info);
    return *this;
}

Status Status::invokeError(QString info)
{
    values.insert("status", "failed");
    values.insert("details", "invoke method failed: " + info);
    return *this;
}

Status Status::parsingError(QString info)
{
    values.insert("status", "failed");
    values.insert("details", " " + info);
    return *this;
}

Status::Status(Response *parentAction) :
    parentAction(parentAction)
{

}
