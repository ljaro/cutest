#include "invoke.h"

#include <cstring>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QMetaObject>
#include <QGenericReturnArgument>
#include <QJsonArray>
#include <memory>
using namespace TestRobot::Action;
//TODO make " for string logs and leave without for numbers and bool
Invoke::Invoke()
{

}

QGenericArgument convertArg(const QVariant& val, int& argInt, bool& argBool, QString& argQString, double& argDouble)
{
    if (val.type() == QVariant::Type::Double)
    {
        argDouble = val.toDouble();
        return Q_ARG(bool, argDouble);
    }
    else if (val.type() == QVariant::Type::Int)
    {
        argInt = val.toInt();
        return Q_ARG(int, argInt);
    }
    else if (val.canConvert<bool>())
    {
        argBool = val.toBool();
        return Q_ARG(bool, argBool);
    }
    else if(val.canConvert<QString>())
    {
        argQString = val.toString();
        return Q_ARG(QString, argQString);
    }

    return {};
}

bool Invoke::execSync(TestObject context, ActionCallback callback)
{
    if(!context.isValid())
    {
        callback(context, ActionStatus::create(this).contextNotFound());
        return true;
    }

    if(method_name.isEmpty())
    {
        callback(context, ActionStatus::create(this).invokeError("Method name is empty"));
        return true;
    }

    if(paramList.size() > 5)
    {
        callback(context, ActionStatus::create(this).invokeError("Too much params. Max is 5"));
        return true;
    }

    auto qobj = context.getQObject();

    //this line guarantee that invokeMethod will execute with directConnection since we are on main thread
    if(QThread::currentThread() != qApp->thread())
    {
        // global error
        callback(context, ActionStatus::create(this).invokeError("Test action's thread is not main thread!"));
        return true;
    }

    if(QThread::currentThread() != qobj->thread())
    {
        callback(context, ActionStatus::create(this).invokeError("Context's thread is not main thread!"));
        return true;
    }

    auto bytes = method_name.toLocal8Bit();
    auto methodStr = bytes.data();

    if(paramList.size() == 0)
    {
        bool result = QMetaObject::invokeMethod(context.getQObject(), methodStr, Qt::DirectConnection);
        callback(context, result ? ActionStatus::create(this).ok() : ActionStatus::create(this).invokeError(QString("Cannot execute: %1").arg(methodStr)));
    }

    int argInt1;bool argBool1;QString argQString1;double argDouble1;
    int argInt2;bool argBool2;QString argQString2;double argDouble2;
    int argInt3;bool argBool3;QString argQString3;double argDouble3;
    int argInt4;bool argBool4;QString argQString4;double argDouble4;
    int argInt5;bool argBool5;QString argQString5;double argDouble5;

    if(paramList.size() == 1)
    {
        auto arg1 = convertArg(paramList[0], argInt1, argBool1, argQString1, argDouble1);
        bool result = QMetaObject::invokeMethod(context.getQObject(), methodStr, Qt::DirectConnection, arg1);
        callback(context, result ? ActionStatus::create(this).ok() :
                                   ActionStatus::create(this).invokeError(QString("Cannot execute: %1(%2)").arg(methodStr, paramList[0].toString())));
    }

    if(paramList.size() == 2)
    {
        QGenericArgument arg1 = convertArg(paramList[0], argInt1, argBool1, argQString1, argDouble1);
        QGenericArgument arg2 = convertArg(paramList[1], argInt2, argBool2, argQString2, argDouble2);

        Q_ASSERT(context.getQObject()->thread() == QThread::currentThread());

        bool result = QMetaObject::invokeMethod(context.getQObject(), methodStr, Qt::DirectConnection,
                arg1, arg2
                );
        callback(context, result ? ActionStatus::create(this).ok() :
                                   ActionStatus::create(this).invokeError(QString("Cannot execute: %1(%2, %3)").arg(methodStr, paramList[0].toString(), paramList[1].toString())));
        return true;
    }

    if(paramList.size() == 3)
    {
        auto arg1 = convertArg(paramList[0], argInt1, argBool1, argQString1, argDouble1);
        auto arg2 = convertArg(paramList[1], argInt2, argBool2, argQString2, argDouble2);
        auto arg3 = convertArg(paramList[2], argInt3, argBool3, argQString3, argDouble3);
        bool result = QMetaObject::invokeMethod(context.getQObject(), methodStr, Qt::DirectConnection, arg1, arg2, arg3);
        callback(context, result ? ActionStatus::create(this).ok() :
                                   ActionStatus::create(this)
                                   .invokeError(QString("Cannot execute: %1(%2, %3, %4)")
                                                .arg(methodStr,
                                                     paramList[0].toString(), paramList[1].toString(), paramList[2].toString())));
        return true;
    }

    if(paramList.size() == 4)
    {
        auto arg1 = convertArg(paramList[0], argInt1, argBool1, argQString1, argDouble1);
        auto arg2 = convertArg(paramList[1], argInt2, argBool2, argQString2, argDouble2);
        auto arg3 = convertArg(paramList[2], argInt3, argBool3, argQString3, argDouble3);
        auto arg4 = convertArg(paramList[3], argInt4, argBool4, argQString4, argDouble4);
        bool result = QMetaObject::invokeMethod(context.getQObject(), methodStr, Qt::DirectConnection, arg1, arg2, arg3, arg4);
        callback(context, result ? ActionStatus::create(this).ok() :
                                   ActionStatus::create(this).invokeError(QString("Cannot execute: %1(%2, %3, %4, %5)")
                                                                          .arg(methodStr, paramList[0].toString(), paramList[1].toString(), paramList[2].toString(), paramList[3].toString())));
        return true;
    }

    if(paramList.size() == 5)
    {
        auto arg1 = convertArg(paramList[0], argInt1, argBool1, argQString1, argDouble1);
        auto arg2 = convertArg(paramList[1], argInt2, argBool2, argQString2, argDouble2);
        auto arg3 = convertArg(paramList[2], argInt3, argBool3, argQString3, argDouble3);
        auto arg4 = convertArg(paramList[3], argInt4, argBool4, argQString4, argDouble4);
        auto arg5 = convertArg(paramList[4], argInt5, argBool5, argQString5, argDouble5);
        bool result = QMetaObject::invokeMethod(context.getQObject(), methodStr, Qt::DirectConnection, arg1, arg2, arg3, arg4, arg5);
        callback(context, result ? ActionStatus::create(this).ok() :
                                   ActionStatus::create(this).invokeError(QString("Cannot execute: %1(%2, %3, %4, %5, %6)")
                                                                                                            .arg(methodStr, paramList[0].toString(), paramList[1].toString(),
                 paramList[2].toString(), paramList[3].toString(), paramList[4].toString())));
        return true;
    }

    callback(context, ActionStatus::create(this).notImplemented());

    return true;
}

AsyncResult Invoke::execAsync(TestObject context, ActionCallback callback)
{
    Q_UNUSED(context)
    Q_UNUSED(callback)
    return {};
}

QVariant convertToNumber(const QJsonValue& value)
{
    bool hasDot = value.toString().contains(".") || value.toString().contains(",");
    if(hasDot && value.isDouble())
    {
        return QVariant(value.toDouble());
    }
    else
    {
        auto var = value.toVariant();

        if(var.type() == QVariant::Type::Bool)
        {
            return QVariant::fromValue(value.toBool());
        }
        else
        {
            bool ok = false;
            value.toString().toInt(&ok, 10);
            if(ok)
            {
                return QVariant::fromValue(value.toInt());
            }
            else {
                return QVariant::fromValue(value.toString());
            }
        }
    }

    return value.toVariant();
}

//TODO apply params method should validate params and do failure callback before exec
void Invoke::applyParams(QJsonObject params)
{
    method_name = params.value("method").toString();
    paramList.clear();

    if(!params.value("arg1").isUndefined())
        paramList.append(convertToNumber(params.value("arg1")));

    if(!params.value("arg2").isUndefined())
        paramList.append(convertToNumber(params.value("arg2")));

    if(!params.value("arg3").isUndefined())
        paramList.append(convertToNumber(params.value("arg3")));

    if(!params.value("arg4").isUndefined())
        paramList.append(convertToNumber(params.value("arg4")));

    if(!params.value("arg5").isUndefined())
        paramList.append(convertToNumber(params.value("arg5")));
}
