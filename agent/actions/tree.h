#ifndef TREE_H
#define TREE_H
#include <QQuickItem>
#include <QJsonObject>

#include "testaction.h"

namespace TestRobot {
namespace Action {

class Tree : public TestAction
{
public:
    Tree();
public:
    std::string className() const override {return "tree";}

    // TestAction interface
protected:
    bool execSync(TestObject context, ActionCallback callback) override;
    AsyncResult execAsync(TestObject context, ActionCallback callback) override;
    void applyParams(QJsonObject params) override;
private:
    QJsonObject quickItemToJsonObject(QQuickItem *item) const;
};

}}
#endif // TREE_H
