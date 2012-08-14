QT += network

TEMPLATE = lib
CONFIG += dll
DESTDIR = $$PWD/../bin/lib

HEADERS += \
    packet.h \
    message.h \
    ircconstants.h \
    ircconnection.h \
    firc.h \
    Singleton.h \
    messagehandler.h

SOURCES += \
    packet.cpp \
    message.cpp \
    ircconnection.cpp \
    firc.cpp \
    messagehandler.cpp
