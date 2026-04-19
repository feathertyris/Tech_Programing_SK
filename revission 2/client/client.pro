QT += core gui network widgets charts

CONFIG += c++11


TARGET = ClientGUI
TEMPLATE = app


SOURCES += \
    main.cpp \
    clientgui.cpp


HEADERS += \
    clientgui.h

FORMS += \
    clientgui.ui

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += charts
}
