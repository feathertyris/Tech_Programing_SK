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
    projectdescriptiondialog.cpp \
    recoverydialog.cpp \
    registrationdialog.cpp \
    tokenpassworddialog.cpp

HEADERS += \
    authdialog.h \
    clientgui.h \
    projectdescriptiondialog.h \
    recoverydialog.h \
    registrationdialog.h \
    tokenpassworddialog.h

FORMS += \
    authdialog.ui \
    clientgui.ui \
    projectdescriptiondialog.ui \
    recoverydialog.ui \
    registrationdialog.ui \
    tokenpassworddialog.ui

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += charts
}
