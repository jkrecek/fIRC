QT       += core network
QT       -= gui

TARGET = fIRCsrv
CONFIG   += console
CONFIG   -= app_bundle
CONFIG(debug, debug|release):DESTDIR = $$PWD/../bin/debug
else:DESTDIR = $$PWD/../bin/release

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h

INCLUDEPATH += ../shared
LIBS += -L"$$PWD/../bin/lib" -lshared
