#include <QApplication>
#include "clientgui.h"
#include "authdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AuthDialog authDialog;
    if (authDialog.exec() == QDialog::Accepted) {
        ClientGUI client;
        client.setUsername(authDialog.getUsername()); // Передаём имя пользователя
        client.show();
        return a.exec();
    } else {
        return 0; // Пользователь отменил авторизацию
    }
}
