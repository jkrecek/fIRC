#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTextStream>
#include <QList>
#include <QPair>
#include <QHostAddress>
#include <QHash>
#include <QSettings>

#include <socket.h>

class QTcpServer;
class QTcpSocket;

class Server : public QObject
{
    Q_OBJECT

    public:
        Server(QHostAddress addr = QHostAddress(QHostAddress::Any), int port = 2266);

    private:
        QTcpServer* server;
        QTextStream cerr;
        QHash<QTcpSocket*, QString> logins;
        QHash<QString, int> log;

        QSettings settings;

        void login(QTcpSocket* socket, const QString& user, const QString& pass, bool force = false);

    private slots:
        void purgeLog();
        void handleNewConnection();
        void reply();
        void gotDisconnected();
};

#endif // SERVER_H
