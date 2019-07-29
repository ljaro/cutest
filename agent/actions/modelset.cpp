#include "modelset.h"

#include <QAbstractItemModel>

using namespace TestRobot::Action;

ModelSet::ModelSet()
{

}

bool ModelSet::execSync(TestObject context, ActionCallback callback)
{
    if(auto model = qobject_cast<QAbstractItemModel*>(context.getQObject()))
    {
        auto role = model->roleNames().key(paramRole.toUtf8(), Qt::DisplayRole);

        if(index.isValid())
        {
            bool result = model->setData(model->index(index.toInt(), 0), value, role);
            if(result)
            {
                callback(context, ActionStatus::create(this).ok());
            }
            else
            {
                callback(context, ActionStatus::create(this).invokeError("setData return false"));
            }
        }
        else
        {
            callback(context, ActionStatus::create(this).wrongParams("index not found"));
        }

    }
    else
    {
        callback(context, ActionStatus::create(this).notImplemented("object must be QAbstractItemModel"));
    }

    return true;
}

AsyncResult ModelSet::execAsync(TestObject context, ActionCallback callback)
{
    return {};
}

void ModelSet::applyParams(QJsonObject params)
{
    paramRole = params.value("role").toString();
    value = params.value("value").toVariant();
    index = params.value("index").toInt(-1);
}
