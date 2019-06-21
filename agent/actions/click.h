#ifndef CLICK_H
#define CLICK_H
#include <vector>
#include <functional>
#include "testaction.h"
#include "testobject.h"
#include "params/paramprops.h"

namespace TestRobot {
namespace Action {

class Click : public TestAction
{
public:
    Click();

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    QSharedPointer<QMetaObject::Connection> execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
    std::string className() const override {return "click";}
private:
    std::shared_ptr<ParamProps> params;
};

}
}

#endif // CLICK_H
