#ifndef MODELSET_H
#define MODELSET_H

#include "testaction.h"

namespace TestRobot {
namespace Action {

class ModelSet : public TestAction
{
public:
    ModelSet();

    // TestAction interface
public:
    std::string className() const override {return "model_set";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;

    QString paramRole;
    QVariant index;
    QVariant value;
};

}}

#endif // MODELSET_H
