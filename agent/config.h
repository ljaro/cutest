#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QJsonObject>
#include <vector>

class Config
{
public:
    Config();
    static Config& instance();
    QString server_ip = "0.0.0.0";
    QString server_port = "10010";
private:
    std::vector<QString> paths = {"./testconfig.json","~/testconfig.json", "/usr/local/tests/testconfig.json"};
    bool load();
    bool fileExists(QString path);
    QString selectConfigFile();
    QString replaceTilde(QString path);
    QVariant getValue(const QString& key, const QJsonObject& obj);
    bool assignValue(const QJsonObject &jsonObj, QString &dest, const QString &key);
    QJsonObject readFromFile();
};

#endif // CONFIG_H
