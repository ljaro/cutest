#include "cache.h"
#include <iostream>
#include <QMutexLocker>
#include <QQuickItem>
#include <QWidget>
#include <QCoreApplication>

#include "inspector.h"

QList<QQuickItem*> visualChildren(QQuickItem* ancestor)
{
    auto result = ancestor->childItems();

    const auto tmpItems = result;
    for(const auto& item : tmpItems)
    {
        result.append(visualChildren(item));
    }

    return result;
}

Cache::Cache() : mutex(QMutex::Recursive)
{

}

Cache::~Cache()
{
    std::cout << "~Cache";
}

Cache::Cache(const Cache& other)
{
    QMutexLocker locker(&other.mutex);
    objects = other.objects;
}

Cache::Cache(const QObjectList& objects)
    : objects(objects.toSet())
{

}

Cache::Cache(const QSet<QObject *> &objects)
    : objects(objects)
{

}

Cache& Cache::operator=(const Cache& rhs)
{
    this->objects = rhs.objects;
    return *this;
}

bool Cache::hasVisualObject(QObject *obj)
{
    QMutexLocker locker(&mutex);
    return lookupObjectName.contains(obj) && objects.contains(obj);
}

bool Cache::hasObject(QObject *obj)
{
    QMutexLocker locker(&mutex);
    return objects.contains(obj);
}

bool Cache::insert(QObject *obj)
{
    QMutexLocker locker(&mutex);

    if(obj->thread() != qApp->thread()) {
        return false;
    }

    objects.insert(obj);

    if(filter(obj)) {
        return false;
    }


    //std::cout << "thread " << static_cast<void*>(obj->thread()) << std::endl;

    auto className = Inspector::getClassName(obj);
    auto objName = Inspector::getObjectName(obj);
    auto typeName = Inspector::getObjectTypeName(obj);

    objectNameHash.insertMulti(objName, obj);
    classNameHash.insertMulti(className, obj);
    typeNameHash.insertMulti(typeName, obj);

    lookupObjectName.insert(obj, objName);
    lookupObjectClass.insert(obj, className);
    lookupObjectType.insert(obj, typeName);

    return true;
}

bool Cache::remove(QObject *obj)
{
    QMutexLocker locker(&mutex);

    objects.remove(obj);
    objectNameHash.remove(lookupObjectName.value(obj), obj);
    classNameHash.remove(lookupObjectClass.value(obj), obj);
    typeNameHash.remove(lookupObjectType.value(obj), obj);

    return true; //TODO x.remove returns
}

void Cache::insertObjectTree(QObject *obj)
{
    if(!obj) {
        return;
    }

    if(!objects.contains(obj))
    {
        objects.insert(obj);

        auto className = Inspector::getClassName(obj);
        auto objName = Inspector::getObjectName(obj);
        auto typeName = Inspector::getObjectTypeName(obj);

        objectNameHash.insertMulti(objName, obj);
        classNameHash.insertMulti(className, obj);
        typeNameHash.insertMulti(typeName, obj);

        lookupObjectName.insert(obj, objName);
        lookupObjectClass.insert(obj, className);
        lookupObjectType.insert(obj, typeName);
    }

    for(auto obj : qAsConst(obj->children()))
    {
        insertObjectTree(obj);
    }
}

bool Cache::matchObjectName(QObject *obj, const QString &name)
{
    auto objName = Inspector::getObjectName(obj);
    if(name.contains("*"))
    {
        QRegExp rx(name);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        if(rx.exactMatch(objName))
            return true;
    }
    else
    {
        if(objName == name)
            return true;
    }

    return false;
}

bool Cache::matchClassName(QObject *obj, const QString &name)
{
    auto objName = Inspector::getClassName(obj);
    if(name.contains("*"))
    {
        QRegExp rx(name);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        if(rx.exactMatch(objName))
            return true;
    }
    else
    {
        if(objName == name)
            return true;
    }

    return false;
}

void Cache::dump()
{
//    for(const auto& p: qAsConst(lookupObjectClass))
//    {
//       // if(!p.isEmpty() && p != "none")
//       // std::cout << ":" << p.toStdString() << std::endl;
//    }
}

void Cache::refreshCache(QObject *root)
{
    QMutexLocker locker(&mutex);

    objectNameHash.clear();
    classNameHash.clear();
    typeNameHash.clear();

    lookupObjectName.clear();
    lookupObjectClass.clear();
    lookupObjectType.clear();

    objects.clear();

    insertObjectTree(root);
}

Cache Cache::intersect(const Cache &cache)
{
    Cache newCache(*this);
    newCache.objects.intersect(cache.objects);
    return newCache;
}

Cache Cache::unite(const Cache &cache)
{
    Cache newCache(*this);
    newCache.objects.unite(cache.objects);
    return newCache;
}

bool Cache::isEmpty() const
{
    return objects.isEmpty();
}

Cache Cache::children(QObject *ancestor)
{
    if(!ancestor)
    {
        return Cache();
    }

    QObjectList lst;
    if(auto qancestor = qobject_cast<QQuickItem*>(ancestor))
    {
        auto ch = qancestor != nullptr ? visualChildren(qancestor) : QList<QQuickItem*>();

        lst.reserve(ch.size());
        for (auto obj : qAsConst(ch))
        {
            lst.append(obj);
        }
    }
    else if(qobject_cast<QWidget*>(ancestor))
    {
        auto ch = ancestor != nullptr ? ancestor->findChildren<QWidget*>() : QList<QWidget*>();
        lst.reserve(ch.size());
        for (auto obj : qAsConst(ch))
        {
            lst.append(obj);
        }
    }

    return Cache(lst);
}

Cache Cache::byObjectName(const QString& name) const
{
    QMutexLocker locker(&mutex);

    if(name.contains("*"))
    {
        QObjectList res;
        QRegExp rx(name);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        for(const auto& objectName : objectNameHash.keys())
        {
            if(rx.exactMatch(objectName))
            {
                res.append(objectNameHash.values(objectName).toSet().intersect(objects).toList());
            }
        }
        return Cache(res);
    }
    else
    {
        auto const& lst = objectNameHash.values(name).toSet().intersect(objects).toList();
        return Cache(lst);
    }
}

Cache Cache::byObjectName(QObject *ancestor, const QString& name) const
{
    QObjectList result;
    bySearchInAncestor(ancestor, name, result, Cache::matchObjectName);
    return Cache(result);
}

Cache Cache::byClassName(const QString &name) const
{
    QMutexLocker locker(&mutex);

    if(name.contains("*"))
    {
        QObjectList res;
        QRegExp rx(name);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        for(const auto& objectName : classNameHash.keys())
        {
            if(rx.exactMatch(objectName))
            {
                res.append(classNameHash.values(objectName).toSet().intersect(objects).toList());
            }
        }
        return Cache(res);
    }
    else
    {
        auto const& lst = classNameHash.values(name).toSet().intersect(objects).toList();
        return Cache(lst);
    }
}

Cache Cache::byClassName(QObject *ancestor, const QString &name) const
{
    QObjectList result;
    bySearchInAncestor(ancestor, name, result, Cache::matchClassName);
    return Cache(result);
}

template <typename Func>
void Cache::bySearchInAncestor(QObject *ancestor, const QString& name, QObjectList &result, Func func) const
{
    if(auto qancestor = qobject_cast<QQuickItem*>(ancestor))
    {
        if(!qancestor)
        {
            std::cout << "QObject ancestor is null" << std::endl;
        }

        auto children = qancestor != nullptr ? visualChildren(qancestor) : QList<QQuickItem*>();

        for (const auto& child : qAsConst(children))
        {
            if(func(child, name))
            {
                result.append(child);
            }
        }
    }
    else if(qobject_cast<QWidget*>(ancestor))
    {
        auto children = ancestor != nullptr ? ancestor->findChildren<QWidget*>() : QList<QWidget*>();
        for (const auto& child : qAsConst(children))
        {
            if(func(child, name))
            {
                result.append(child);
            }
        }
    }
}

/**
 * @brief Cache::byPropertyValue
 *
 * Be aware that this method is search in whole cache.
 * Its fine if cache is filtered previously
 * TODO - maybe should be guard on first cache or limits of objects
 *
 * @param property
 * @param value
 * @return new filtered Cache object
 */
Cache Cache::byPropertyValue(const QString &property, const QVariant &value) const
{
    QSet<QObject*> result;
    for(auto const& obj : qAsConst(objects))
    {
        auto propertyValue = Inspector::getPropertyValue(obj, property);

        if(propertyValue.canConvert(QMetaType::QString) && value.canConvert(QMetaType::QString))
        {
            auto regexStr = value.toString();
            QRegExp rx(regexStr);
            rx.setPatternSyntax(QRegExp::WildcardUnix);
            if(rx.exactMatch(propertyValue.toString()))
            {
                result.insert(obj);
            }
        }

    }

    return Cache(result);
}

bool Cache::filter(QObject* obj)
{
    return  !(qobject_cast<QQuickItem*>(obj) || qobject_cast<QWidget*>(obj));
}

