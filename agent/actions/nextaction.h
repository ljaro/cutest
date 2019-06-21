#ifndef NEXT_H
#define NEXT_H

#include "testaction.h"
namespace TestRobot {
namespace Action {
class Next : public TestAction
{
public:
    Next();

    // TestAction interface
public:
    std::string className() const override {return "next";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
};
}}
#endif // NEXT_H
