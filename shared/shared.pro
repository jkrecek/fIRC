QT += network

TEMPLATE = lib
CONFIG += dll
DESTDIR = $$PWD/../bin/lib

HEADERS += \
    socket.h \
    packet.h \
    messageparser.h \
    message.h \
    ircconstants.h \
    ircconnection.h

SOURCES += \
    socket.cpp \
    packet.cpp \
    messageparser.cpp \
    message.cpp \
    ircconnection.cpp
