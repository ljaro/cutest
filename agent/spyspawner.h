#ifndef SPYSPAWNER_H
#define SPYSPAWNER_H

#include <QObject>

class SpySpawner : public QObject
{
    Q_OBJECT
public:
    explicit SpySpawner(QObject *parent = nullptr);
public slots:
    void spawnSpy();
};

#endif // SPYSPAWNER_H
