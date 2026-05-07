QT -= gui
QT += network
QT += core network sql
CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
        database.cpp \
        main.cpp \
        math_engine.cpp \
        postgresqlserver.cpp

HEADERS += \
    auth.h \
    database.h \
    math_engine.h \
    postgresqlserver.h
