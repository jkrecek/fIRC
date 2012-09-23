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

    namespace RemoteConnectionMethod
    {
        static const QByteArray New = "NEW";
        static const QByteArray Connect = "CONNECT";
    }

    namespace Ports
    {
        static const quint16 DefaultIRC = 6667;
        static const quint16 DefaultRemote = 2266;
    }

    namespace Connection
    {
        static const QString ConnectionList = "connections";
        static const QString ConnectionPrefix = "con_";
        static const QString Method = "method";
        static const QString Address = "address";
        static const QString Channels = "channels";
        static const QString Nick = "nick";
        static const QString User = "user";
        static const QString Pass_Hash = "pass_hash";

        namespace ConnectionMethod
        {
            static const QString Local = "local";
            static const QString Remote = "remote";
        }
    }
}

#endif // FIRC_H
