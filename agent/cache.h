#ifndef CACHE_H
#define CACHE_H
#include <QObject>
#include <QMutex>
#include <QSet>
#include <QObjectList>

#include "cachelookup.h"

/**
 * @brief The Cache class
 *
 * Keep all living objects
 * It's fine to copy object of this class as it makes shallow copy of living objects
 */
class Cache
{
public:
    Cache();
    ~Cache();
    Cache(const Cache& other);
    Cache(const QObjectList& objects);
    Cache(const QSet<QObject*>& objects);
    Cache& operator=(const Cache& rhs);

    bool hasVisualObject(QObject* obj);
    bool hasObject(QObject* obj);
    bool insert(QObject* obj);
    bool remove(QObject* obj);
    void dump();
    void refreshCache(QObject* root);

    // access
    QObjectList getObjects() { return objects.values();}
    Cache intersect(const Cache& cache);
    Cache unite(const Cache& cache);
    bool isEmpty() const;
    // search
    Cache children(QObject* ancestor);
    Cache byObjectName(const QString& name) const;
    Cache byObjectName(QObject *ancestor, const QString& name) const;
    Cache byClassName(const QString& name) const;
    Cache byClassName(QObject *ancestor, const QString& name) const;
    Cache byPropertyValue(const QString& property, const QVariant& value) const;
private:
    mutable QMutex mutex;
    bool filter(QObject *obj);
    void insertObjectTree(QObject* obj);
private:
    QSet<QObject*> objects;

    //TODO was refactored.. later remove initialization
    QMultiHash<QString, QObject*>& objectNameHash = CacheLookup::instance().objectNameHash;
    QMultiHash<QString, QObject*>& classNameHash = CacheLookup::instance().classNameHash;
    QMultiHash<QString, QObject*>& typeNameHash = CacheLookup::instance().typeNameHash;
    QHash<QObject*, QString>& lookupObjectName = CacheLookup::instance().lookupObjectName;
    QHash<QObject*, QString>& lookupObjectClass = CacheLookup::instance().lookupObjectClass;
    QHash<QObject*, QString>& lookupObjectType = CacheLookup::instance().lookupObjectType;

    static bool matchObjectName(QObject *obj, const QString &name);
    static bool matchClassName(QObject *obj, const QString &name);

    template<typename Func>
    void bySearchInAncestor(QObject *ancestor, const QString& name, QObjectList &result, Func func) const;
};

#endif // CACHE_H
