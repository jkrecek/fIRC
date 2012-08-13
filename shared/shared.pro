QT += network

TEMPLATE = lib
CONFIG += dll
DESTDIR = $$PWD/../bin/lib

HEADERS += \
    packet.h \
    messageparser.h \
    message.h \
    ircconstants.h \
    ircconnection.h \
    messagebuilder.h \
    firc.h \
    Singleton.h

SOURCES += \
    packet.cpp \
    messageparser.cpp \
    message.cpp \
    ircconnection.cpp \
    messagebuilder.cpp \
    firc.cpp \
