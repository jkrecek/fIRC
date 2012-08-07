#include "channeluser.h"

#include <QStringList>

ChannelUser::ChannelUser(QString userName, Rank rank) :
    userName_m(userName),
    rank_m(rank)
{
}

QList<ChannelUser> ChannelUser::ParseUserList(QString line)
{
    QList<ChannelUser> l;
    QStringList users = line.split(' ');
    foreach (QString user, users)
    {
        Rank r = RANK_NONE;
        QChar firstChar = user.at(0);
        if (firstChar == QChar(126))
            r = RANK_OWNER;
        else if (firstChar == QChar(38))
            r = RANK_ADMIN;
        else if (firstChar == QChar(64))
            r = RANK_OPERATOR;
        else if (firstChar == QChar(37))
            r = RANK_HALF_OPERATOR;
        else if (firstChar == QChar(43))
            r = RANK_VOICE;

        if (r != RANK_NONE)
            user.remove(0, 1);

        l.push_back(ChannelUser(user, r));
    }
    return l;
}

