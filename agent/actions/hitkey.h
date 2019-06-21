#ifndef HITKEY_H
#define HITKEY_H

#include "testaction.h"

namespace TestRobot {
namespace Action {
class HitKey : public TestAction
{
public:
    HitKey();

    // TestAction interface
public:
    std::string className() const override {return "hit_key";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
private:
    QString keyName;
};
}}
#endif // HITKEY_H
