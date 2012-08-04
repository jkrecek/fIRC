#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QStringList>
#include <QTextCodec>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int port = 2266;
    QHostAddress addr(QHostAddress::Any);

    a.setApplicationName("fIRC");
    a.setOrganizationDomain("firc.cz");

    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );

    Server s(addr, port);

    return a.exec();
}
