#ifndef SET_H
#define SET_H

#include "testaction.h"
namespace TestRobot {
namespace Action {
class Set : public TestAction
{
public:
    Set();

    // TestAction interface
public:
    std::string className() const override {return "set";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;

    QString property_name;
    QVariant property_value;
};
}}
#endif // SET_H
