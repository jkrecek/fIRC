#include "messagebuilder.h"

#define APPLY(apply) QByteArray(apply ? "+" : "-")

QString RankHandler::toString(Rank rank)
{
    switch (rank)
    {
        case RANK_OWNER:            return "q";
        case RANK_ADMIN:            return "a";
        case RANK_OPERATOR:         return "o";
        case RANK_HALF_OPERATOR:    return "h";
        case RANK_VOICE:            return "v";
        default:                    return "";
    }
}

QByteArray MessageBuilder::Join(const QString &channel)
{
    return "JOIN " + channel.toUtf8();
}

QByteArray MessageBuilder::Message(const QString &target, const QString &content)
{
    return "PRIVMSG " + target.toUtf8() + " :" + content.toUtf8();
}

QByteArray MessageBuilder::Action(const QString &target, const QString &content)
{
    return Message(target, QString(char(1)) + "ACTION " + content + char(1));
}

QByteArray MessageBuilder::Notice(const QString &user, const QString &content)
{
    return "NOTICE " + user.toUtf8() + " :" + content.toUtf8();
}

QByteArray MessageBuilder::ChangeNick(const QString& newNick)
{
    return "NICK " + newNick.toUtf8();
}

QByteArray MessageBuilder::Mode(const QString& channel, const QString& parameters)
{
    return "MODE " + channel.toUtf8() + " "+ parameters.toUtf8();
}

QByteArray MessageBuilder::GiveRank(const QString &channel, const QString &nick, Rank rank, bool apply)
{
    return Mode(channel, APPLY(apply) + RankHandler::toString(rank) + " " + nick.toUtf8());
}

QByteArray MessageBuilder::Kick(const QString& nick, const QString& channel, const QString& reason)
{
    return "KICK " + channel.toUtf8() + " " + nick.toUtf8() + (!reason.isEmpty() ? (" :") + reason.toUtf8() : "");
}
