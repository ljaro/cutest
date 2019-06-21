#ifndef SPY_H
#define SPY_H

#include <QObject>
#include <QAtomicPointer>
#include <QMutex>
#include <QVector>
#include <QTimer>
#include "cache.h"

class Spy : public QObject
{
    Q_OBJECT
private:
    explicit Spy(QObject *parent = nullptr);
    static QAtomicPointer<Spy> m_instance;
    bool filter(QObject *obj) const;

    QVector<QObject*> newObjectsQueue;
    QTimer *m_queueTimer = nullptr;
    QObject* rootWindow = nullptr;

    void notifyQueue();
    Q_INVOKABLE void processQueue();
public:
    Cache cache;
    static void create();
    static Spy* instance();
    static QMutex *objectLock();
    static QObject* getRootWindow() { return instance()->rootWindow;}

    void onObjectCreated(QObject *obj);
    void onObjectDeleted(QObject *obj);

signals:
    void objectCreated(QObject *obj);
    void objectDeleted(QObject *obj);
public slots:
    void windowAfterRendering();
};

#endif // SPY_H
