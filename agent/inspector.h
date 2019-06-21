#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QString>
#include <QObject>

class Inspector
{
public:
    Inspector();
    static QString getObjectName(QObject *object);
    static QString getObjectTypeName(QObject *object);
    static QString getClassName(QObject *object);
    static QString getSuperClassName(QObject *object);
    static QVariant getPropertyValue(QObject *object, QString propName);
    static bool hasProperty(QObject *object, QString propName);
    static QObject *getVisualParent(QObject *object);
};

#endif // INSPECTOR_H
