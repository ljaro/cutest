#include "spyspawner.h"
#include "spy.h"
#include <QCoreApplication>
#include <QThread>
SpySpawner::SpySpawner(QObject *parent) : QObject(parent)
{
    moveToThread(QCoreApplication::instance()->thread());
    QMetaObject::invokeMethod(this, "spawnSpy", Qt::QueuedConnection);
}

void SpySpawner::spawnSpy()
{
    if (!qApp) {
        //TODO log
        deleteLater();
        return;
    }

    Q_ASSERT(QThread::currentThread() == qApp->thread());

    //TODO tbd
    Spy::create();

    deleteLater();
}
