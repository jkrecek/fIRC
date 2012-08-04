#include "server.h"

#include <QtNetwork>
#include <cstdio>

Server::Server(QHostAddress addr, int port) : cerr(stderr, QIODevice::WriteOnly)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));

    QTimer::singleShot(60*1000, this, SLOT(purgeLog()));

    server->setProxy(QNetworkProxy::NoProxy);
    server->setMaxPendingConnections(50);

    bool listening = server->listen(addr, port);

    QString ipport = addr.toString() + ":" + QString::number(port);
    if (!listening)
    {
        cerr << tr("couldn't bind to %1, quitting…").arg(ipport) + "\n";
        cerr.flush();

        QTimer::singleShot(0, qApp, SLOT(quit()));
    }
    else
    {
        cerr << tr("listening on %1").arg(ipport) + "\n";
        cerr.flush();
    }

}

void Server::purgeLog()
{
    log.clear();
}

void Server::handleNewConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    if (!socket)
        return;

    cerr << tr("incoming connection from ") << socket->peerAddress().toString() << "\n";
    cerr.flush();

    connect(socket, SIGNAL(readyRead()), SLOT(reply()));
    connect(socket, SIGNAL(disconnected()), SLOT(gotDisconnected()));

    Socket::write(socket, OPC_LOGIN);
}

void Server::reply()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    Packet packet(socket);
    QString user = logins.value(socket);

    qDebug() << "handling data, opcode:" << packet.opcode() << ", data: " << packet.data();

    if (packet.opcode() == OPC_LOGIN || packet.opcode() == OPC_FORCELOGIN)
    {
        if (!user.isEmpty())
        {
            Socket::write(socket, OPC_ALREADYLOGGED);
            return;
        }

        QList<QByteArray> args = packet.data().split(' ');

        QString pass;

        if (args.size() == 2)
        {
            user = args.at(0);
            pass = args.at(1);
        }

        login(socket, user, pass, packet.opcode() == OPC_FORCELOGIN);
        return;
    }

    if (user.isEmpty())
    {
        QString ip(socket->peerAddress().toString());

        socket->disconnectFromHost();

        cerr << tr("a client (IP=%1) has requested something without being logged in,").arg(ip)
             << "\n" << tr("which is suspicious – closing the connection.") << "\n";
        cerr.flush();
    }

    switch (packet.opcode())
    {
        default:
            Socket::write(socket, OPC_INVALIDCMD);

            cerr << "invalid opcode: " << packet.opcode() << "\n";
            cerr.flush();

            return;
    }
}


void Server::login(QTcpSocket* socket, const QString& user, const QString& pass, bool force)
{
    qDebug() << "loguju neasi";
    qDebug() << "user " + user;
    qDebug() << "pass " + pass;
    QString setting_str = QString("logins/%1").arg(user);
    bool isPresent = settings.contains(setting_str);
    if (isPresent)
    {
        QString savedpass = settings.value(setting_str).toString();
        if (pass == savedpass)
        {
            Socket::write(socket, OPC_LOGGED);

            logins.insert(socket, user);

            cerr << tr("user %1 successfully logged in").arg(user) << "\n";
            cerr.flush();
        }
        else
        {
            Socket::write(socket, OPC_WRONGLOGIN);

            socket->disconnectFromHost();

            QString ip(socket->peerAddress().toString());
            log.insert(ip, log.value(ip, 0)+1);

            cerr << tr("user %1 tried to log in with a wrong password, "
                       "closing the connection").arg(user) << "\n";
            cerr.flush();
        }
    }
    else
    {
        settings.setValue(setting_str, pass);

        Socket::write(socket, OPC_LOGGED);

        logins.insert(socket, user);

        cerr << tr("user %1 successfully logged in").arg(user) << "\n";
        cerr.flush();
    }
}

void Server::gotDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>( sender() );

    QString user = logins.value(socket);

    if (user.isEmpty())
        user = "*not logged*";
    else
        logins.remove(socket);

    socket->deleteLater();

    cerr << tr("a client (%1) has disconnected").arg(user) << "\n";
    cerr.flush();
}
