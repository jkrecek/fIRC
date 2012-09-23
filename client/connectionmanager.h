#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QList>
#include <connectionmanager.h>

#include <QString>
#include <QStringList>
#include <QSettings>

#include <Singleton.h>

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
            ConnectionDetails(_label, CON_REMOTE), m_address(_address), m_user(_user), m_pass_hash(_pass_hash)
        {}

        const QString m_address;
        const QString m_user;
        const QString m_pass_hash;
};

class ConnectionManager
{
    public:
        ConnectionManager();
        ~ConnectionManager();

        void loadConnections();
        void saveConnection(ConnectionDetails* details);
        void deleteConnection(ConnectionDetails* connection, bool permanent);

        QString getGroupPrefix(QString connectionLabel);

        QMap<QString, ConnectionDetails*> getConnections() const { return m_connections; }
        bool hasConnection(QString connLabel) const { return m_connections.contains(connLabel); }
        ConnectionDetails* getConnection(QString connLabel) const { return m_connections.value(connLabel); }



    private:
        QMap<QString, ConnectionDetails*> m_connections;
        QMap<QString, ConnectionDetails*> m_connections_deleted;

        QSettings settings;
};

#define sConnections Singleton<ConnectionManager>::Instance()

#endif // CONNECTIONMANAGER_H
