#ifndef INVOKE_H
#define INVOKE_H

#include "testaction.h"

namespace TestRobot {
namespace Action {
class Invoke : public TestAction
{
public:
    Invoke();

    // TestAction interface
public:
    std::string className() const override {return "invoke";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
private:
    QString method_name;
    QVariantList paramList;
};
}}
#endif // INVOKE_H
