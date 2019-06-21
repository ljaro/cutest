#include "waitforchild.h"

#include <QSharedPointer>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include "testobject.h"
#include "params/paramexpr.h"
#include "spy.h"

using namespace TestRobot::Action;
using namespace std;

WaitForChild::WaitForChild()
{

}

bool WaitForChild::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    if(!params)
    {
        callback(TestObject(), ActionStatus::create(this).wrongParams());
        return true;
    }

    auto const& expParams = std::static_pointer_cast<ParamExpr>(params);
    QObjectList results;

    // filter cache to children of context
    auto cache = Spy::instance()->cache.children(context.getQObject());

    expParams->matchParam(cache, results);
    if(!results.empty())
    {
        if(results.size() > 0)
        {
            qDebug() << "results > 0";
        }

        auto oneResult = results.first();
        auto testObject = TestObject(oneResult);
        callback(testObject, ActionStatus::create(this).ok());
        return true;
    }

    return false;
}

//TODO handle setParent/ changing parent
AsyncResult WaitForChild::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)

    auto const& expParams = std::static_pointer_cast<ParamExpr>(params);
    auto con = QSharedPointer<QMetaObject::Connection>::create();
    auto handler = [=](QObject* obj)
    {
        Q_ASSERT(QThread::currentThread() == qApp->thread());

        auto cache = Spy::instance()->cache.children(context.getQObject());

        if(obj)
        {
            QObjectList lst;
            auto found = expParams->matchParam(cache, lst);
            if(found)
            {
                QObject::disconnect(*con);
                //TODO only one result from callback?
                callback(TestObject(obj), ActionStatus::create(this).ok());
            }
        }
    };

    *con = QObject::connect(Spy::instance(), &Spy::objectCreated, handler);

    return con;
}

void WaitForChild::applyParams(QJsonObject params)
{
    this->params = std::shared_ptr<ParamExpr>();
    if(!params.isEmpty())
    {
        auto exp = params.value("exp").toString().toStdString();
        auto result = make_shared<ParamExpr>(exp);
        this->params = result;
    }
    else
    {
        qWarning() << "Dispatcher::getQueryParams - cannot find json object 'params'";
    }
}
