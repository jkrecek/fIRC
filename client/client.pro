QT       += network

TARGET = fIRCcli
CONFIG   -= app_bundle
CONFIG(debug, debug|release):DESTDIR = $$PWD/../bin/debug
else:DESTDIR = $$PWD/../bin/release

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    firctab.cpp \
    channeluser.cpp \
    widgets/ircconnectionselectdialog.cpp

HEADERS += \
    mainwindow.h \
    firctab.h \
    channeluser.h \
    widgets/ircconnectionselectdialog.h

FORMS += mainwindow.ui \
    widgets/ircconnectionselectdialog.ui

CODECFORTR = UTF-8

INCLUDEPATH += ../shared
LIBS += -L"$$PWD/../bin/lib" -lshared
