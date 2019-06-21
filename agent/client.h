#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSet>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QNetworkSession>
#include <QTimer>
#include "remoteclient.h"
#include "remotehandler.h"

class Client : public RemoteHandler
{
    Q_OBJECT
public:
    Client(QTcpSocket* socket);
    void sendResponse(const QString &response) override;
private:
    QTcpSocket* socket;
    QDataStream in;
    void read();
    void write(QByteArray data);
    void stateChanged(QAbstractSocket::SocketState state);
    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // CLIENT_H
