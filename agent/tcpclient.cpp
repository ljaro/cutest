#include "tcpclient.h"
#include <QThread>
#include <QCoreApplication>
#include <iostream>
#include <limits>
#include "config.h"

TcpClient::TcpClient(QObject* parent) :
    RemoteClient(parent),
    socket(this)
{
    socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    in.setDevice(&socket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(&socket, &QTcpSocket::readyRead, this, &TcpClient::read);
    connect(&socket, &QTcpSocket::stateChanged, this, &TcpClient::stateChanged);
    connect(&socket, &QTcpSocket::connected, this, &TcpClient::connected);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpClient::displayError);
}

void TcpClient::start()
{
    if(!socket.isOpen())
    {
        QString ip = Config::instance().server_ip;
        auto port = Config::instance().server_port.toUShort();

        std::cout << "trying connecting " << ip.toStdString() << ":" << port << std::endl;
        socket.connectToHost(ip, port);
    }
}

void TcpClient::sendResponse(const QString &response)
{    
    write(response.toUtf8());
}

void TcpClient::connected()
{
    qDebug() << "connected";
}

void TcpClient::read()
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

void TcpClient::write(QByteArray data)
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

void TcpClient::stateChanged(QAbstractSocket::SocketState state)
{

}

void TcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "The host close connection.";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. " \
                    "Make sure the server is running, " \
                    "and check that the host name and port " \
                    "settings are correct.";
        break;
    default:
        qDebug() << "The following error occurred: " << socket.errorString();
    }

    socket.abort();
    socket.close();
    QTimer::singleShot(3000, [this](){start();});
}

