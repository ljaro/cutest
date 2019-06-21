#ifndef REMOTEHANDLER_H
#define REMOTEHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkSession>
#include <QTimer>

class RemoteHandler : public QObject
{
    Q_OBJECT
public:
    RemoteHandler();
    virtual void sendResponse(const QString& response) = 0;
signals:
    void commandReceived(const QString&);
};

#endif // REMOTEHANDLER_H
