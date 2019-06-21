#include "testaction.h"
#include <QMutexLocker>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <vector>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <memory>
#include "spy.h"
#include "params/paramexpr.h"
#include "inspector.h"
#include "actionresult.h"
#include "actions/waitfor.h"
#include "actions/click.h"
#include "actions/nullaction.h"
#include "actions/mousepress.h"
#include "actions/mouserelease.h"
#include "actions/mousemove.h"
#include "actions/highlight.h"
#include "actions/write.h"

#include "params/paramprops.h"
#include "params/paramexpr.h"

using namespace std;


void TestActionBuilder::registerAction(std::shared_ptr<TestAction> action, QString className)
{
    std::string classNm = className.isEmpty() ? action->className() : className.toStdString();
    TestAction::getActionNamesMap().insert(make_pair(classNm, action));
}

TestAction& TestActionBuilder::getAction(QString name, TestObject context, QJsonObject params)
{
    Q_UNUSED(context)

    auto& m = TestAction::getActionNamesMap();
    auto it = m.find(name.toStdString());
    if(it != m.end())
    {
        auto& action = *(it->second);
        action.applyParams(params);
        return action;
    }
    else
    {
        return *m.at("null");
    }
}

void TestAction::exec(TestObject context, ActionCallback callback)
{
    QMutexLocker locker(Spy::objectLock());

    if(!execSync(context, callback))
    {
        auto conn = execAsync(context, callback);

        if(conn)
        {
            auto connPtr = conn.toWeakRef();
            QTimer::singleShot(getTimeout(), [connPtr, this, context, callback](){

                auto conn = connPtr.toStrongRef();
                if(conn)
                {
                    QObject::disconnect(*conn);
                    timeoutHandler(context, callback);
                }
            });
        }
    }


    //TODO make flag callback called and check it here
}

void TestAction::timeoutHandler(TestObject context, ActionCallback callback)
{
    qDebug() << "timeout handler";
    callback(TestObject(context), ActionStatus::create(this).globalTimeout());
}

int TestAction::getTimeout() const
{
    constexpr int PARAM_TIMEOUT = 6000;
    return PARAM_TIMEOUT;
}

TestAction::ActionNamesMap &TestAction::getActionNamesMap()
{
    static ActionNamesMap map;
    return map;
}

