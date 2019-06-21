#ifndef WAITFOR_H
#define WAITFOR_H
#include <vector>
#include <functional>
#include "testaction.h"
#include "testobject.h"
#include "params/paramexpr.h"
namespace TestRobot {
namespace Action {
class WaitFor : public TestAction
{
public:
    WaitFor();
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "wait_for";}
private:
    std::shared_ptr<ParamExpr> params;
};
}}
#endif // WAITFOR_H
