#include <QApplication>
#include "authdialog.h"
#include "projectdescriptiondialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Шаг 1: Показываем диалог авторизации
    AuthDialog authDialog;
    if (authDialog.exec() == QDialog::Accepted) {
        // Шаг 2: После успешной авторизации показываем окно с описанием проекта
        ProjectDescriptionDialog descDialog;
        descDialog.exec();  // Модальное отображение — ждём закрытия окна описания
    } else {
        // Если авторизация не пройдена — завершаем приложение
        return 0;
    }

    return app.exec();
}
