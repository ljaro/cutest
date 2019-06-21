#ifndef WRITE_H
#define WRITE_H

#include "testaction.h"
namespace TestRobot {
namespace Action {
class Write : public TestAction
{
public:
    Write();

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "write";}
private:
    QString text;
};
}}
#endif // WRITE_H
