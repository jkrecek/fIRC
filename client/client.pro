QT       += network

TARGET = fIRCcli
CONFIG   -= app_bundle
CONFIG(debug, debug|release):DESTDIR = $$PWD/../bin/debug
else:DESTDIR = $$PWD/../bin/release

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += mainwindow.ui
CODECFORTR = UTF-8

INCLUDEPATH += ../shared
LIBS += -L"$$PWD/../bin/lib" -lshared
