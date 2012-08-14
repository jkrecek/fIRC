#ifndef IRCCONSTANTS_H
#define IRCCONSTANTS_H

#include <QString>

namespace IRC
{
    namespace Command
    {
        static const QByteArray Ping = "PING";
        static const QByteArray PrivMsg = "PRIVMSG";
        static const QByteArray Notice = "NOTICE";
        static const QByteArray Join = "JOIN";
        static const QByteArray Kick = "KICK";
        static const QByteArray Part = "PART";
        static const QByteArray Topic = "TOPIC";
        static const QByteArray Error = "ERROR";
        static const QByteArray Mode = "MODE";
        static const QByteArray Nick = "NICK";
    }

    namespace Reply
    {
        static const QString WELCOME = "001";
        static const QString TOPIC = "332";
        static const QString TOPIC_SET = "333";
        static const QString NAMREPLY = "353";
        static const QString END_CHANNEL_DATA = "366";
        static const QString MOTD = "372";        
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
