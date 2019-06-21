#include "nextaction.h"

#include <QTimer>
#include <QQuickItem>
#include <QWidget>
#include <QThread>
#include <spy.h>

using namespace TestRobot::Action;

Next::Next()
{

}

bool Next::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    auto qobject = context.getQObject();
    if(QQuickItem* item = qobject_cast<QQuickItem*>(qobject))
    {
        if(item->thread() == QThread::currentThread())
        {
            auto nextItem = item->nextItemInFocusChain();
            if(nextItem)
            {
                callback(TestObject(nextItem), ActionStatus::create(this).ok());
            }

            return true;
        }
    }
    else if(QWidget* item = qobject_cast<QWidget*>(qobject))
    {
        auto nextItem = item->nextInFocusChain();
        if(nextItem)
        {
            callback(TestObject(nextItem), ActionStatus::create(this).ok());
            return true;
        }
    }

    callback(context, ActionStatus::create(this).objectNotFound());

    return true;
}

AsyncResult Next::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void Next::applyParams(QJsonObject params)
{

}

