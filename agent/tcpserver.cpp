#include "tcpserver.h"
#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <iostream>
#include <limits>
#include "config.h"
#include "client.h"

TcpServer::TcpServer(QObject* parent) :
    RemoteClient(parent),
    server(this)
{
    in.setVersion(QDataStream::Qt_4_0);

    connect(&server, &QTcpServer::newConnection, this, &TcpServer::connected);
}

TcpServer::~TcpServer()
{
    qDeleteAll(clients);
}

void TcpServer::connected()
{
    auto socket = server.nextPendingConnection();
    auto client = new Client(socket);

    if(clients.contains(client))
    {
        qWarning() << "next connection already exists";
    }

    clients.insert(client);

    emit clientConnected(client);
}

void TcpServer::start()
{
    if(!server.isListening())
    {
        QString ip = Config::instance().server_ip;
        auto port = Config::instance().server_port.toUShort();

        std::cout << "listening... " << ip.toStdString() << ":" << port << std::endl;

        QHostAddress addr(ip);
        if(!server.listen(addr, port))
        {
            std::cout << server.errorString().toStdString() << " " << ip.toStdString() << ":" << port << std::endl;
        }
    }
}

void TcpServer::sendResponse(const QString &response)
{
    QTcpSocket socket;
    write(socket, response.toUtf8());
}

void TcpServer::read(QTcpSocket& socket)
{
    in.startTransaction();

    constexpr auto buffSize = 1024;
    char buff[buffSize] = {0};

    auto bytes = static_cast<int>(socket.bytesAvailable());
    in.readRawData(buff, std::min(bytes, buffSize));
    QString result;
    result = buff;

    if(!in.commitTransaction())
        return;

    emit commandReceived(result);
}

void TcpServer::write(QTcpSocket& socket, QByteArray data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    auto size = data.size();

    if(size >= std::numeric_limits<unsigned short>::max())
    {
        qDebug() << "message size should not exceed " << std::numeric_limits<unsigned short>::max();
        return;
    }

    out << static_cast<quint16>(size);
    out << data;
    socket.write(block);
}

void TcpServer::stateChanged(QAbstractSocket::SocketState state)
{

}



