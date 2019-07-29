#include "client.h"

Client::Client(QTcpSocket *socket) : socket(socket)
{
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_4_0);

    QObject::connect(socket, &QTcpSocket::readyRead, this, &Client::read);
    QObject::connect(socket, &QTcpSocket::stateChanged, this, &Client::stateChanged);
    QObject::connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                     this, &Client::displayError);
}

void Client::sendResponse(const QString &response)
{
    qDebug() << "sendResponse";
    write(response.toUtf8());
}

void Client::read()
{
    in.startTransaction();

    constexpr auto buffSize = 1024;
    char buff[buffSize] = {0};

    auto bytes = static_cast<int>(socket->bytesAvailable());
    in.readRawData(buff, std::min(bytes, buffSize-1));
    QString result;
    result = buff+2;


    if(!in.commitTransaction())
        return;

    emit commandReceived(result);
}

void Client::write(QByteArray data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_4_0);

    auto size = data.size();

    if(size >= 16000000)
    {
        qDebug() << "message size should not exceed " << std::numeric_limits<unsigned short>::max();

        //TODO handle response
        return;
    }

    out << static_cast<quint8>((size >> 16) & 0xFF);
    out << static_cast<quint8>((size >> 8) & 0xFF);
    out << static_cast<quint8>(size & 0xFF);

    out.writeRawData(data.data(), data.size());

    qDebug() << block;
    qDebug() << socket->write(block);
}

void Client::stateChanged(QAbstractSocket::SocketState state)
{

}

void Client::displayError(QAbstractSocket::SocketError socketError)
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
        qDebug() << "The following error occurred: " << socket->errorString();
    }

    socket->abort();
    socket->close();

}
