#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QSet>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QNetworkSession>
#include <QTimer>
#include "remoteclient.h"

class Client;
class TcpServer : public RemoteClient
{
    Q_OBJECT
public:
    TcpServer(QObject* parent);
    ~TcpServer() override;
    void start() override;
    void sendResponse(const QString &response) override;
private:
    QTcpServer server;
    QSet<Client*> clients;
    QDataStream in;

    QString sessionId();
    void read(QTcpSocket& socket);
    void write(QTcpSocket& socket, QByteArray data);
    void stateChanged(QAbstractSocket::SocketState state);
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();
signals:
    void clientConnected(Client*);
};

#endif // TCPCLIENT_H
