#include "tree.h"

#include <QQuickItem>
#include <QJsonArray>
#include "inspector.h"

using namespace TestRobot::Action;

Tree::Tree()
{

}

QJsonObject Tree::quickItemToJsonObject(QQuickItem * item) const
{
    QJsonObject result;
    result.insert("objectName", Inspector::getObjectName(item));
    result.insert("className", Inspector::getClassName(item));
    result.insert("typeName", Inspector::getObjectTypeName(item));
    return result;
}

bool Tree::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    auto qobject = context.getQObject();
    auto quickItem = qobject_cast<QQuickItem*>(qobject);

    std::function<void(QQuickItem*, QJsonObject&)> printChildren;

    printChildren = [&](QQuickItem* parent, QJsonObject& parentJsonObject) {

        QJsonArray children;
        for(auto const& item : parent->childItems())
        {
            auto thisItem = quickItemToJsonObject(item);
            printChildren(item, thisItem);
            children.append(thisItem);
        }

        parentJsonObject.insert("children", children);

    };

    QJsonObject root = quickItemToJsonObject(quickItem);
    printChildren(quickItem, root);

    ActionResult result;
    result.type = ActionResult::ValueType::Simple;
    result.simpleValue = root;
    callback(result, ActionStatus::create(this).ok());

    return true;
}

AsyncResult Tree::execAsync(TestObject context, ActionCallback callback)
{
    return {};
}

void Tree::applyParams(QJsonObject params)
{

}
