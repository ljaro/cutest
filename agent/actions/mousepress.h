#ifndef MOUSEPRESS_H
#define MOUSEPRESS_H

#include "testaction.h"
#include <QVariant>
#include <QPoint>
namespace TestRobot {
namespace Action {
class MousePress : public TestAction
{
public:
    MousePress();

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "mouse_press";}
private:
    QVariant x, y;
    QPoint lastMove;

    enum class ActionType {
        None,
        Press,
        Move,
        Release
    };

    ActionType actionType = ActionType::None;
};
}}
#endif // MOUSEPRESS_H
