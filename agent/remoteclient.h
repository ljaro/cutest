#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QNetworkSession>
#include <QTimer>

class RemoteClient : public QObject
{
    Q_OBJECT
public:
    RemoteClient(QObject* parent);
    virtual void start() = 0;
    virtual void sendResponse(const QString& response) = 0;
signals:
    void commandReceived(QString);
};

#endif // REMOTECLIENT_H
