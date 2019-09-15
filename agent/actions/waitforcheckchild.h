#ifndef WAITFORCHECKCHILD_H
#define WAITFORCHECKCHILD_H

#include "waitforchild.h"
namespace TestRobot {
namespace Action {
class WaitForCheckChild : public WaitForChild
{
public:
    WaitForCheckChild();
protected:
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "wait_check_child";}
    void timeoutHandler(TestObject context, ActionCallback callback) override;
    int getTimeout() const override;

    int timeout = 0;
};
}}
#endif // WAITFORCHECKCHILD_H
