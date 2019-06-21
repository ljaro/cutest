#ifndef ROLENAMES_H
#define ROLENAMES_H

#include "testaction.h"

namespace TestRobot {
namespace Action {

class RoleNames : public TestAction
{
public:
    RoleNames();

    // TestAction interface
public:
    std::string className() const override {return "role_names";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
};

}}

#endif // ROLENAMES_H
