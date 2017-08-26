#include "server.h"
#include <iostream>

Server::Server()
{
    connect(&server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

bool Server::start(uint port)
{
    if(server.isListening()) return 1;
    if(server.listen(QHostAddress::Any, port)){
        std::cout << "server started at " << port << " port" << std::endl;
        return 1;
    }
    std::cout << "server was not started at " << port << " port" << std::endl;
    return 0;
}

void Server::stop()
{
    if (server.isListening())
        server.close();
}

void Server::acceptConnection()
{
    QSslSocket * socket = dynamic_cast<QSslSocket*>(sender());

    // QSslSocket emits the encrypted() signal after the encrypted connection is established
    connect(socket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));

    // Report any SSL errors that occur
    connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)),
            this, SLOT(sslErrors(const QList<QSslError> &)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionFailure()));

    std::cout << "connection accepted from " << socket->peerAddress().toString().toStdString()
              << ":" << socket->peerPort() << " (" << socket->peerName().toStdString() << ")"
              << std::endl;

    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket->startServerEncryption();
}

void Server::handshakeComplete()
{
    QSslSocket * socket = dynamic_cast<QSslSocket*>(sender());

    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));

    std::cout << "handshake complete from " << socket->peerAddress().toString().toStdString()
              << ":" << socket->peerPort() << " (" << socket->peerName().toStdString() << ")"
              << std::endl;
}

void Server::receiveMessage()
{
    QSslSocket * socket = dynamic_cast<QSslSocket*>(sender());

    std::cout << "new data from " << socket->peerAddress().toString().toStdString()
              << ":" << socket->peerPort() << " (" << socket->peerName().toStdString()
              << ") size:" << socket->bytesAvailable() << "byte(s) :" << std::endl;
    while(socket->bytesAvailable())
            std::cout << socket->readAll().constData();

    std::cout << socket->readLine().constData();
    std::cout << "--END--" << std::endl;
}

void Server::connectionClosed()
{
    QSslSocket * socket = dynamic_cast<QSslSocket*>(sender());

    std::cout << "connection closed " << socket->peerAddress().toString().toStdString()
              << ":" << socket->peerPort() << " (" << socket->peerName().toStdString()
              << ") because: " << socket->errorString().toStdString() << std::endl;
    socket->disconnect();
    socket->deleteLater();
}

void Server::sslErrors(const QList<QSslError> &errors)
{
    QSslSocket * socket = dynamic_cast<QSslSocket*>(sender());

    std::cout << "ssl erron(s) on " << socket->peerAddress().toString().toStdString()
              << ":" << socket->peerPort() << " (" << socket->peerName().toStdString()
              << ") because: " << std::endl;

    foreach (QSslError error, errors)
       std::cout << "   *" << error.errorString().toStdString() << std::endl;
}

void Server::connectionFailure()
{
    QSslSocket * socket = dynamic_cast<QSslSocket*>(sender());

    std::cout << "connection failure " << socket->peerAddress().toString().toStdString()
              << ":" << socket->peerPort() << " (" << socket->peerName().toStdString()
              << ") because: " << socket->errorString().toStdString() << std::endl;
    socket->disconnect();
    socket->deleteLater();
}
