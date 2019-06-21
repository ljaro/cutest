#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <QQuickItem>

#include "testaction.h"

namespace TestRobot {
namespace Action {

class Highlight : public TestAction
{
public:
    Highlight();

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "highlight";}
private:
    int mseconds = 0;
};

}}

#endif // HIGHLIGHT_H
