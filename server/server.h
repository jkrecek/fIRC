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
        void estabilishIRCconnectionForUser(QByteArray host, QList<QByteArray> channels, User* user);

        QTcpServer* server;
        QTextStream cerr;
        QHash<QString, int> log;

        QSettings settings;

        QHash<QTcpSocket*, User*> user_conns_m;
        QHash<User*, IRCconnection*> IRC_conns_m;
};

#endif // SERVER_H
