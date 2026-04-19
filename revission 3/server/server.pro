QT -= gui
QT += network
QT += core network sql
CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    database.cpp \
    main.cpp \
    mytcpserver.cpp

HEADERS += \
    database.h \
    mytcpserver.h

DISTFILES += \
    init_database.sql
