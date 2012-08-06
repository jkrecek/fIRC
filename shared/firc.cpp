#include "firc.h"

bool fIRC::isFromGlobalChannel(QString channel)
{
    if (channel.isEmpty())
        return true;

    if (channel.startsWith(":"))
        return true;

    //static QString globalChannels = { "nickserv", "hopm-siglost", "chanserv", "py-ctcp" };
    //foreach (QString gChannel, globalChannels)
    //    if (channel == gChannel)
    //        return true;
    static QString globalChannels = "|nickserv|hopm-siglost|chanserv|py-ctcp|";
    if (globalChannels.contains("|" + channel + "|"))
        return true;

    return false;
}
