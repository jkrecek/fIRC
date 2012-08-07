#ifndef FIRC_H
#define FIRC_H

#include <QString>

namespace fIRC
{
    bool isFromGlobalChannel(QString channel);
    QString addHyperLinks(QString content);
}

#endif // FIRC_H
