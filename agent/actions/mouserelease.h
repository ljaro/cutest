#ifndef MOUSERELEASE_H
#define MOUSERELEASE_H


#include "testaction.h"
namespace TestRobot {
namespace Action {
class MouseRelease : public TestAction
{
public:
    MouseRelease();

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "mouse_release";}
};
}}
#endif // MOUSERELEASE_H
