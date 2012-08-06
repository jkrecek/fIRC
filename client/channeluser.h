#ifndef CHANNELUSER_H
#define CHANNELUSER_H

#include <QList>
#include <messagebuilder.h>

class ChannelUser
{
    public:
        ChannelUser(QString userName, Rank rank);

        static QList<ChannelUser> ParseUserList(QString line);

    private:
        QString userName_m;
        Rank rank_m;
};

#endif // CHANNELUSER_H
