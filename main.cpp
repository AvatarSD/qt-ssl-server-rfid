#include <QCoreApplication>
#include <QSslSocket>
#include <server.h>

#define USE_PORT 443

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.loadCertAndKey("ca.crt", "ca.key");
    server.start(USE_PORT);

    return a.exec();
}
