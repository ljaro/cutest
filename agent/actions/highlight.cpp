#include "highlight.h"
#include <QQuickItem>
#include <QRectF>
#include <QPointF>
#include <QTimer>

#include "overlaypainter.h"
using namespace TestRobot::Action;

Highlight::Highlight()
{

}

bool Highlight::execSync(TestObject context,
                               ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return false; // false pass to execAsync
}

AsyncResult Highlight::execAsync(TestObject context,
                                ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return {};
    }

    auto qobject = qobject_cast<QQuickItem*>(context.getQObject());
    OverlayPainter::instance().highlightItem(qobject, mseconds, [=](){
        callback(TestObject(context), ActionStatus::create(this).ok());
    });

    return {};
}

void Highlight::applyParams(QJsonObject params)
{
    mseconds = params.value("msec").toInt();
}
