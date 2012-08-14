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
    widgets/ircconnectionselectdialog.cpp \
    widgets/ircconnectioncreatedialog.cpp \

HEADERS += \
    mainwindow.h \
    firctab.h \
    channeluser.h \
    widgets/ircconnectionselectdialog.h \
    widgets/ircconnectioncreatedialog.h

FORMS += mainwindow.ui \
    widgets/ircconnectionselectdialog.ui \
    widgets/ircconnectioncreatedialog.ui

CODECFORTR = UTF-8

INCLUDEPATH += ../shared
LIBS += -L"$$PWD/../bin/lib" -lshared
