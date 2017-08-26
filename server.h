#ifndef SERVER_H
#define SERVER_H

#include "sslserver.h"


class Server : public QObject
{
    Q_OBJECT

public:
    Server();

    bool start(uint port);
    void stop();

protected slots:
    /* working slots to receive signals from sockets */
    void acceptConnection();
    void handshakeComplete();
    void receiveMessage();

    /* notifiing signals from sockets */
    void connectionClosed();
    void sslErrors(const QList<QSslError> &errors);
    void connectionFailure();

private:
    SslServer server;
};

#endif // SERVER_H
