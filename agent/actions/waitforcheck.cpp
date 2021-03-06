#include "waitforcheck.h"
#include "actionresult.h"
using namespace TestRobot::Action;

WaitForCheck::WaitForCheck()
{

}

void WaitForCheck::applyParams(QJsonObject params)
{
    WaitFor::applyParams(params);

    timeout = params.value("timeout").toInt();
}

void WaitForCheck::timeoutHandler(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)

    ActionResult result;
    result.type = ActionResult::ValueType::Simple;
    result.simpleValue = QVariant::fromValue(false);

    callback(result, ActionStatus::create(this).ok());
}

int WaitForCheck::getTimeout() const
{
    return timeout;
}
