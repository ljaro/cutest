#ifndef FINDSIBLING_H
#define FINDSIBLING_H

#include "testaction.h"
#include <vector>
#include <functional>

namespace TestRobot {
namespace Action {

class FindSibling : public TestAction
{
public:
    FindSibling();

    // TestAction interface
public:
    std::string className() const override {return "find_sibling";}

protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
private:
    std::shared_ptr<ParamExpr> expr;
    QString ancestorObjectName;
    QObject *getAncestor(TestObject context);
};

}}

#endif // FINDSIBLING_H
