#ifndef NULL_H
#define NULL_H
#include "testaction.h"
namespace TestRobot {
namespace Action {
class Null : public TestAction
{
public:
    Null();
    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    QSharedPointer<QMetaObject::Connection> execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "null";}
};
}}
#endif // NULL_H
