#include "findsibling.h"
#include "params/paramexpr.h"
#include <QDebug>
#include "spy.h"
#include "inspector.h"
using namespace TestRobot::Action;
using namespace std;
FindSibling::FindSibling()
{

}

QObject* FindSibling::getAncestor(TestObject context)
{
    if(ancestorObjectName.isEmpty())
    {
        return Inspector::getVisualParent(context.getQObject());
    }

    auto ancestorExpr = ParamExpr(QString("className=%1").arg(ancestorObjectName).toStdString());


    QObject* visualParent = Inspector::getVisualParent(context.getQObject());
    while(visualParent)
    {
        if(ancestorExpr.matchParam(visualParent))
        {
            break;
        }
        visualParent = Inspector::getVisualParent(visualParent);
    }

    return visualParent;
}

bool FindSibling::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    auto ancestor = getAncestor(context);
    if(ancestor)
    {
        auto const& expParams = std::static_pointer_cast<ParamExpr>(expr);

        auto cache = Spy::instance()->cache.children(ancestor);
        QObjectList results;
        expParams->matchParam(cache, results);
        if(!results.empty())
        {
            if(results.size() > 1)
            {
                callback(TestObject(), ActionStatus::create(this).moreObjectsFound());
                return true;
            }

            auto oneResult = results.first();
            auto testObject = TestObject(oneResult);
            callback(testObject, ActionStatus::create(this).ok());
            return true;
        }
        else {
            callback(TestObject(), ActionStatus::create(this).objectNotFound());
            return true;
        }
    }
    else
    {
        callback(TestObject(), ActionStatus::create(this).ancestorNotFound());
        return true;
    }
}

AsyncResult FindSibling::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void FindSibling::applyParams(QJsonObject params)
{
    this->expr = std::shared_ptr<ParamExpr>();
    if(!params.isEmpty())
    {
        auto exp = params.value("exp").toString().toStdString();
        auto result = make_shared<ParamExpr>(exp);
        this->expr = result;

        ancestorObjectName = params.value("ancestor").toString();
    }
    else
    {
        qWarning() << "Dispatcher::getQueryParams - cannot find json object 'params'";
    }
}

