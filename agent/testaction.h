#ifndef TEST_H
#define TEST_H
#include <QObject>
#include <qobjectdefs.h>
#include <QJsonObject>
#include <QSharedPointer>
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include "param.h"
#include "testobject.h"
#include "testactionstatus.h"
#include "response.h"
#include "actionresult.h"
typedef std::function<void(ActionResult, Status)> ActionCallback;
typedef QSharedPointer<QMetaObject::Connection> AsyncResult;

class ParamExpr;
class TestAction;

class TestActionBuilder
{
public:
    static void registerAction(std::shared_ptr<TestAction>, QString className = QString());
    static TestAction& getAction(QString name, TestObject context, QJsonObject params);
    TestActionBuilder() = delete;
};

class TestAction : public Response
{
    friend class TestActionBuilder;
protected:
    TestAction() = default;
    virtual ~TestAction() = default;
public:
    void exec(TestObject context, ActionCallback callback);
    virtual std::string className() const = 0;
protected:

    virtual bool execSync(TestObject context, ActionCallback callback) = 0;
    virtual AsyncResult execAsync(TestObject context, ActionCallback callback) = 0;
    virtual void applyParams(QJsonObject params) = 0;
    virtual void timeoutHandler(TestObject context, ActionCallback callback);
    virtual int getTimeout() const;
private:
    typedef std::unordered_map<std::string, std::shared_ptr<TestAction>> ActionNamesMap;
    static ActionNamesMap &getActionNamesMap();
};



#endif // TEST_H
