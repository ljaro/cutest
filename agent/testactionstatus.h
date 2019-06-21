#ifndef TESTACTIONSTATUS_H
#define TESTACTIONSTATUS_H

#include <QHash>
#include <QVariantHash>

class Response;

class Status
{
public:
    Status(Response* parentAction);
    Status ok();
    Status wrongParams();
    Status actionNotFound();
    Status contextNotFound();
    Status objectNotFound();
    Status ancestorNotFound();
    Status globalTimeout();
    Status moreObjectsFound();
    Status contextMustBeQQuickItem();
    Status unsupportedType(const QString& typeInfo);
    Status notImplemented(QString info);
    Status invokeError(QString info);

    QVariantHash getStatuses() {return values;}
private:
    Response* parentAction;
    QVariantHash values;
};

/**
 * @brief The ActionStatus class
 * This class is builder for Status objects
 * To distinguish ActionStatus::create() from ActionStatus::create().doSomething()
 */
class ActionStatus
{
public:
    static ActionStatus create(Response* parentAction);
    Status ok() {return Status(parentAction).ok();}
    Status wrongParams() {return Status(parentAction).wrongParams();}
    Status actionNotFound() {return Status(parentAction).actionNotFound();}
    Status contextNotFound() {return Status(parentAction).contextNotFound();}
    Status objectNotFound() {return Status(parentAction).objectNotFound();}
    Status moreObjectsFound() {return Status(parentAction).moreObjectsFound();}
    Status ancestorNotFound() {return Status(parentAction).ancestorNotFound();}
    Status globalTimeout() {return Status(parentAction).globalTimeout();}
    Status contextMustBeQQuickItem() {return Status(parentAction).contextMustBeQQuickItem();}
    Status unsupportedType(QString typeInfo) {return Status(parentAction).unsupportedType(typeInfo);}
    Status notImplemented(QString info = ""){return Status(parentAction).notImplemented(info);}
    Status invokeError(QString info){return Status(parentAction).invokeError(info);}
private:
    Response* parentAction;
    ActionStatus(Response* parentAction);
};

#endif // TESTACTIONSTATUS_H
