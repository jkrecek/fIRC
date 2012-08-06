#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <QString>

enum Rank
{
    RANK_OWNER,
    RANK_ADMIN,
    RANK_OPERATOR,
    RANK_HALF_OPERATOR,
    RANK_VOICE,
    RANK_NONE
};

namespace RankHandler
{
    QString toString(Rank rank);
}

namespace MessageBuilder
{
    QByteArray Join(const QString& channel);
    QByteArray Message(const QString& target, const QString& content);
    QByteArray Action(const QString& target, const QString& content);
    QByteArray Notice(const QString& user, const QString& content);
    QByteArray ChangeNick(const QString& newNick);
    QByteArray Mode(const QString& channel, const QString& parameters);
    QByteArray GiveRank(const QString& channel, const QString& nick, Rank rank, bool apply);
    QByteArray Kick(const QString& nick, const QString& channel, const QString& reason = QString());
}

#endif // MESSAGEBUILDER_H
