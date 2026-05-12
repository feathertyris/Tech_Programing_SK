#include <QCoreApplication> //  базовый класс Qt для консольных приложений (без графического интерфейса)
#include "postgresqlserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv); // Создаётся объект app типа QCoreApplication

    PostgreSQLServer server; // Создание объекта сервера
    return app.exec(); // запуск обработки событий . Завершится только когда кто-то вызовет app.quit()
}
