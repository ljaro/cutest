#include "config.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <iostream>
#include <QVariant>
using namespace std;

Config::Config()
{
    load();
}

Config &Config::instance()
{
    static Config config;
    return config;
}

bool Config::load()
{
    QJsonObject rootObj = readFromFile();
    assignValue(rootObj, server_ip, "server_ip");
    assignValue(rootObj, server_port, "server_port");
    return true;
}

QJsonObject Config::readFromFile()
{
    QString currentPath = selectConfigFile();

    if(currentPath.isEmpty())
    {
        std::cout << "auto tests: config file doesn't exists" << std::endl;
        return QJsonObject();
    }

    QFile file(currentPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "auto tests: config file open error" << std::endl;
        return QJsonObject();
    }

    std::cout << "auto tests: config file: " << currentPath.toStdString() << " opened" << std::endl;

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if(jsonDoc.isNull() || !jsonDoc.isObject())
    {
        return QJsonObject();
    }

    return jsonDoc.object();
}

bool Config::assignValue(const QJsonObject& jsonObj, QString& dest, const QString& key)
{
    if(jsonObj.contains(key))
    {
        QVariant v = jsonObj.value(key);

        if(v.isValid() && !v.toString().isEmpty())
        {
            dest = v.toString();
            return true;
        }
    }

    return false;
}

bool Config::fileExists(QString path)
{
    path = replaceTilde(path);
    QFileInfo file(path);
    return file.exists() && file.isFile();
}

QString Config::selectConfigFile()
{
    for(auto const& configPath : paths)
    {
        if(fileExists(configPath))
        {
            return replaceTilde(configPath);
        }
    }

    return QString();
}

QString Config::replaceTilde(QString path)
{
    if(path.contains("~"))
    {
        path = path.replace("~", QDir::homePath() + QDir::separator());
        path = QDir::cleanPath(path);
    }
    return path;
}

QVariant Config::getValue(const QString& key, const QJsonObject &obj)
{
    if(obj.contains(key))
    {
        return obj.value(key);
    }

    return QVariant();
}


