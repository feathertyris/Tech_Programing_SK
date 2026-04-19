QT += core gui network widgets charts
QT += core gui widgets charts
QT += core network sql
CONFIG += c++11


TARGET = ClientGUI
TEMPLATE = app


SOURCES += \
    authdialog.cpp \
    clientgui.cpp \
    main.cpp \
    recoverydialog.cpp \
    registrationdialog.cpp

HEADERS += \
    authdialog.h \
    clientgui.h \
    recoverydialog.h \
    registrationdialog.h

FORMS += \
    authdialog.ui \
    clientgui.ui \
    recoverydialog.ui \
    registrationdialog.ui

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += charts
}
