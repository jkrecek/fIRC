QT += network

TEMPLATE = lib
CONFIG += dll
DESTDIR = $$PWD/../bin/lib

HEADERS += \
    socket.h

SOURCES += \
    socket.cpp
