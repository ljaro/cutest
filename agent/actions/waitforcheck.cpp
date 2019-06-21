#include "waitforcheck.h"

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
    callback(TestObject(), ActionStatus::create(this).ok());
}

int WaitForCheck::getTimeout() const
{
    return timeout;
}
