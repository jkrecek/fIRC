#ifndef CONNECTIONDETAILS_H
#define CONNECTIONDETAILS_H

#include <QString>

enum ConnectionMethod
{
    CON_LOCAL = 0,
    CON_REMOTE
};

class ConnectionDetails
{
    public:
        ConnectionDetails(QString _label, ConnectionMethod _method) :
            m_label(_label), m_method(_method)
        {}

        const QString m_label;
        const ConnectionMethod m_method;
};

class LocalConnectionDetails : public ConnectionDetails
{
    public:
        LocalConnectionDetails(QString _label, QString _address, QStringList _channels, QString _nick) :
            ConnectionDetails(_label, CON_LOCAL), m_address(_address), m_channels(_channels), m_nick(_nick)
        {}

        const QString m_address;
        const QStringList m_channels;
        const QString m_nick;
};

class RemoteConnectionDetails : public ConnectionDetails
{
    public:
        RemoteConnectionDetails(QString _label, QString _address, QString _user, QString _pass_hash) :
            ConnectionDetails(_label, CON_LOCAL), m_address(_address), m_user(_user), m_pass_hash(_pass_hash)
        {}

        const QString m_address;
        const QString m_user;
        const QString m_pass_hash;
};

#endif // CONNECTIONDETAILS_H
