#include <QCoreApplication>
#include <QSslSocket>
#include <server.h>

#define USE_PORT 1300

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.start(USE_PORT);

    return a.exec();
}
