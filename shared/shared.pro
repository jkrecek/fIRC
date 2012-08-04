QT += network

TEMPLATE = lib
CONFIG += dll
DESTDIR = $$PWD/../bin/lib

HEADERS += \
    socket.h \
    packet.h

SOURCES += \
    socket.cpp \
    packet.cpp
