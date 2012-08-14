#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTextStream>
#include <QList>
#include <QPair>
#include <QHostAddress>
#include <QHash>
#include <QSettings>

#include <packet.h>
#include <ircconnection.h>
#include "userconnection.h"

class QTcpServer;
class QTcpSocket;
class User;

class Server : public QObject
{
    Q_OBJECT

    public:
        Server(QHostAddress addr = QHostAddress(QHostAddress::Any), quint16 port = 2266);

    private slots:
        void purgeLog();
        void handleNewConnection();
        void reply();
        void gotDisconnected();

        void handleReceivedMessage(QByteArray message);

    private:
        void login(QTcpSocket* socket, const QString& user, const QString& pass);
        IRCconnection* newIRCconn(User *user, QByteArray label, QByteArray host, QByteArray nick, QList<QByteArray> channels);

        UserConnection* getConnection(QTcpSocket* socket) const;
        IRCconnection* getIRCConnection(User *user, QByteArray label, QByteArray host, QByteArray nick) const;

        QTcpServer* server;
        QTextStream cerr;
        QHash<QString, int> log;

        QSettings settings;

        QHash<QTcpSocket*, UserConnection*> connections_m;
        QList<QPair<User*, IRCconnection*> > IRC_conns_m;
};

#endif // SERVER_H
