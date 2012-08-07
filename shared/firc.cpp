#include "firc.h"
#include <QDebug>
#include <QRegExp>

bool fIRC::isFromGlobalChannel(QString channel)
{
    if (channel.isEmpty())
        return true;

    if (channel.startsWith(":"))
        return true;

    static QString globalChannels = "|nickserv|hopm-siglost|chanserv|py-ctcp|";
    if (globalChannels.contains("|" + channel + "|"))
        return true;

    return false;
}

QString fIRC::addHyperLinks(QString content)
{
    QString address, newAddress;
    int idx = 0;
    while ((idx = content.toLower().indexOf(QRegExp("https?://"), idx)) != -1)
    {
        address = content.mid(idx, content.indexOf(' ', idx) - idx);
        newAddress = "<a href=\"" + address + "\">" + address + "</a>";
        content.replace(address, newAddress);
        idx += newAddress.size()-1;
    }

    return content;
}
