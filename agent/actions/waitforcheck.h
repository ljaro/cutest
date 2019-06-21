#ifndef WAITFORACTIONCHECK_H
#define WAITFORACTIONCHECK_H

#include "waitfor.h"
namespace TestRobot {
namespace Action {
class WaitForCheck : public WaitFor
{
public:
    WaitForCheck();
protected:
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "wait_for_check";}
    void timeoutHandler(TestObject context, ActionCallback callback) override;
    int getTimeout() const override;

    int timeout = 0;
};
}}
#endif // WAITFORACTIONCHECK_H
