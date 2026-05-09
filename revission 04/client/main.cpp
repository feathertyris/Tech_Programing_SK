#include <QApplication>
#include "authdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AuthDialog authDialog;
    authDialog.show();

    return app.exec();
}
