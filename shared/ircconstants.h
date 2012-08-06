#ifndef IRCCONSTANTS_H
#define IRCCONSTANTS_H

#include <QString>

namespace IRC
{
    namespace Command
    {
        static const QString Ping = "PING";
        static const QString PrivMsg = "PRIVMSG";
        static const QString Notice = "NOTICE";
        static const QString Join = "JOIN";
        static const QString Kick = "KICK";
        static const QString Part = "PART";
        static const QString Topic = "TOPIC";
        static const QString Error = "ERROR";
        static const QString Mode = "MODE";
    }

    namespace Reply
    {
        static const QString WELCOME = "001";
        static const QString TOPIC = "332";
        static const QString MOTD = "372";
        static const QString NAMREPLY = "353";
        static const QString NICKTAKEN = "433";
        static const QString BANNED = "474";
    }

    static const QString MessagePrefix = ":";
    static const QString UserPrefix = "!";
    static const QString HostPrefix = "@";
    static const QString UserIdentifyPrefix = "USER";
    static const QString NickIdentifyPrefix = "NICK";
    static const QString PassIdentifyPrefix = "PASS";

    static const QString ActionPrefix = "/me ";

    static const QByteArray END = "\n";
    static const QString ALT_END = QString(char(10));
}

#endif // IRCCONSTANTS_H
