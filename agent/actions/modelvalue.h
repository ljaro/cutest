#ifndef MODELVALUE_H
#define MODELVALUE_H

#include "testaction.h"

namespace TestRobot {
namespace Action {


class ModelValue : public TestAction
{
public:
    ModelValue();

    // TestAction interface
public:
    std::string className() const override {return "model_value";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
private:
    QString paramRole = "RoleDisplay";
    int paramIndex = 0;
};

}}

#endif // MODELVALUE_H
