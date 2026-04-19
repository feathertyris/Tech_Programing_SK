#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;
    if (!server.startServer()) {
        qCritical() << "Failed to start server";
        return 1;
    }

    return a.exec();
}

