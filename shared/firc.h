#ifndef FIRC_H
#define FIRC_H

#include <QString>

namespace fIRC
{
    bool isFromGlobalChannel(QString channel);
    QString addHyperLinks(QString content);

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
}

#endif // FIRC_H
