#include "hitkey.h"

#include <QTest>
#include <QApplication>

#include "spy.h"
using namespace TestRobot::Action;

HitKey::HitKey()
{

}

bool HitKey::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    static QHash<QString, Qt::Key> keys = {
        {"enter",   Qt::Key_Enter},
        {"backspace",   Qt::Key_Backspace},
        {"home",   Qt::Key_Home},
        {"end",   Qt::Key_End}
    };

    Qt::KeyboardModifiers mod = Qt::NoModifier;
    if(keyName.contains("alt"))
    {
        mod |= Qt::AltModifier;
    }

    if(keyName.contains("control") || keyName.contains("ctrl"))
    {
        mod |= Qt::ControlModifier;
    }

    if(keys.contains(keyName))
    {
        Qt::Key key = keys[keyName];

        auto window = qobject_cast<QWindow*>(Spy::instance()->getRootWindow());
        auto qobject = context.getQObject();

        if(QWidget* widget = qobject_cast<QWidget*>(qobject))
        {
            QTest::keyEvent(QTest::Click, qApp->activeWindow(), key, mod);
        }
        else
        {
            QTest::keyClick(window, key, mod);
        }
    }

    //TODO maybe we should post timer to be sure QTest sent event
    callback(context, ActionStatus::create(this).ok());
    return true;
}

AsyncResult HitKey::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

void HitKey::applyParams(QJsonObject params)
{
    keyName = params.value("key_name").toString();
}
