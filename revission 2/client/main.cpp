#include <QApplication>
#include "clientgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientGUI w;
    w.show();
    return a.exec();
}
