#include "nullaction.h"
#include "param.h"
#include <QDebug>

using namespace TestRobot::Action;

Null::Null()
{

}

bool Null::execSync(TestObject context,
                          ActionCallback callback)
{
    qDebug() << "NullAction has been invoked";
    callback(context, ActionStatus::create(this).actionNotFound());
    return true;
}

AsyncResult Null::execAsync(TestObject context,
                           ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
}

void Null::applyParams(QJsonObject params)
{
    Q_UNUSED(params)
}
