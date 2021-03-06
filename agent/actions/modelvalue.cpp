#include "modelvalue.h"
#include <QAbstractItemModel>
#include <QModelIndex>
using namespace TestRobot::Action;

ModelValue::ModelValue()
{

}

bool ModelValue::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    if(auto model = qobject_cast<QAbstractItemModel*>(context.getQObject()))
    {
        //TODO we don't know what is under value(QVariant)
        //this should be handled with specialized function
        ActionResult result;
        result.type = ActionResult::ValueType::Simple;

        auto role = model->roleNames().key(paramRole.toUtf8(), Qt::DisplayRole);

        if(!paramIndex.isValid())
        {
            QVariantList arr;
            for(int i = 0; i< model->rowCount(); ++i)
            {
                auto value = model->data(model->index(i, 0), role);
                arr.append(value);
            }

            result.simpleValue = arr;
        }
        else
        {
            auto value = model->data(model->index(paramIndex.toInt(), 0), role);
            result.simpleValue = value;
        }

        callback(result, ActionStatus::create(this).ok());
    }
    else
    {
        callback(context, ActionStatus::create(this).notImplemented("object must be QAbstractItemModel"));
    }

    return true;
}

AsyncResult ModelValue::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void ModelValue::applyParams(QJsonObject params)
{
    paramRole = params.value("role").toString();
    paramIndex = params.value("index").toVariant();
}
