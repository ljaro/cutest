#ifndef WAITFORCHILD_H
#define WAITFORCHILD_H

#include "testaction.h"
namespace TestRobot {
namespace Action {
class WaitForChild : public TestAction
{
public:
    WaitForChild();
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "wait_for_child";}
private:
    std::shared_ptr<ParamExpr> params;
};
}}
#endif // WAITFORCHILD_H
