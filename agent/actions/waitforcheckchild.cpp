#include "waitforcheckchild.h"
#include "actionresult.h"
using namespace TestRobot::Action;

WaitForCheckChild::WaitForCheckChild()
{

}

void WaitForCheckChild::applyParams(QJsonObject params)
{
    WaitForChild::applyParams(params);

    timeout = params.value("timeout").toInt();
}

void WaitForCheckChild::timeoutHandler(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)

    ActionResult result;
    result.type = ActionResult::ValueType::Simple;
    result.simpleValue = QVariant::fromValue(false);

    callback(result, ActionStatus::create(this).ok());
}

int WaitForCheckChild::getTimeout() const
{
    return timeout;
}
