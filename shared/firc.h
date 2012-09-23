#ifndef FIRC_H
#define FIRC_H

#include <QString>

namespace fIRC
{
    bool isFromGlobalChannel(QString channel);
    QString addHyperLinks(QString content);
    QString getHashFor(QString pass);

    namespace Position
    {
        static const QByteArray Start = "START";
        static const QByteArray End = "END";
    }

    namespace ConnectionMethod
    {
        static const QByteArray New = "NEW";
        static const QByteArray Connect = "CONNECT";
    }

    namespace Ports
    {
        static const quint16 DefaultIRC = 6667;
        static const quint16 DefaultRemote = 2266;
    }
}

#endif // FIRC_H
