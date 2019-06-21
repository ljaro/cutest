#ifndef MOUSEMOVE_H
#define MOUSEMOVE_H

#include "testaction.h"
namespace TestRobot {
namespace Action {
class MouseMove : public TestAction
{
public:
    MouseMove();

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "mouse_move";}
private:
    int x = 0;
    int y = 0;
};
}}
#endif // MOUSEMOVE_H
