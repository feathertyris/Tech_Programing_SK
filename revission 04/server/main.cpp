#include <QCoreApplication>
#include "postgresqlserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    PostgreSQLServer server;

    return app.exec();
}
