#ifndef GET_H
#define GET_H

#include <QQmlProperty>

#include "testaction.h"

namespace TestRobot {
namespace Action {

class Get : public TestAction
{
public:
    Get();
    std::string className() const override {return "get";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;

private:
    void handleNormal(QQmlProperty prop, ActionCallback callback, TestObject context);
    void handleObject(QQmlProperty prop, ActionCallback callback, TestObject context);
    QString property_name;
};

}}

#endif // GET_H
