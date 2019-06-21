#ifndef CACHELOOKUP_H
#define CACHELOOKUP_H

#include <QHash>
#include <QMultiHash>
#include <QString>
#include <iostream>
class QObject;
class CacheLookup
{
public:
    QMultiHash<QString, QObject*> objectNameHash;
    QMultiHash<QString, QObject*> classNameHash;
    QMultiHash<QString, QObject*> typeNameHash;
    QHash<QObject*, QString> lookupObjectName;
    QHash<QObject*, QString> lookupObjectClass;
    QHash<QObject*, QString> lookupObjectType;

    static CacheLookup& instance();
private:
    CacheLookup(const CacheLookup&) = delete;
    CacheLookup(CacheLookup&&) = delete;
    CacheLookup& operator=(const CacheLookup&) = delete;
    CacheLookup& operator=(CacheLookup&&) = delete;

    CacheLookup() = default;
};

#endif // CACHELOOKUP_H
