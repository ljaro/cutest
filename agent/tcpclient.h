#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QNetworkSession>
#include <QTimer>
#include "remoteclient.h"


class TcpClient : public RemoteClient
{
public:
    TcpClient(QObject* parent);
    void start() override;
    void sendResponse(const QString &response) override;
private:
    QTcpSocket socket;
    QDataStream in;

    QString sessionId();
    void connected();
    void read();
    void write(QByteArray data);
    void stateChanged(QAbstractSocket::SocketState state);
    void displayError(QAbstractSocket::SocketError socketError);
    void clientConnected();
};

#endif // TCPCLIENT_H
