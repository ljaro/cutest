#include "rolenames.h"
#include <QAbstractItemModel>
#include <QModelIndex>
using namespace TestRobot::Action;

RoleNames::RoleNames()
{

}

bool RoleNames::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    if(auto model = qobject_cast<QAbstractItemModel*>(context.getQObject()))
    {

        auto const& val = model->roleNames().values();
        QStringList roleNames;
        roleNames.reserve(val.size());

        for(auto const& v : qAsConst(val))
        {
            roleNames.push_back(v);
        }


        //TODO we don't know what is under value(QVariant)
        //this should be handled with specialized function
        ActionResult result;
        result.type = ActionResult::ValueType::Simple;
        result.simpleValue = roleNames;
        callback(result, ActionStatus::create(this).ok());
    }
    else
    {
        callback(context, ActionStatus::create(this).notImplemented("object must be QAbstractItemModel"));
    }

    return true;
}

AsyncResult RoleNames::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void RoleNames::applyParams(QJsonObject params)
{
    Q_UNUSED(params)
}
