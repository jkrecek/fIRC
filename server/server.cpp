#include "server.h"

#include <QtNetwork>
#include <cstdio>
#include <ircconstants.h>
#include <messagebuilder.h>
#include "usermgr.h"

Server::Server(QHostAddress addr, quint16 port) :
    cerr(stderr, QIODevice::WriteOnly),
    settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationDomain(), QCoreApplication::applicationName())
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
        cerr << "Couldn't bind to " + ipport + "quitting ..." + "\n";
        cerr.flush();

        QTimer::singleShot(0, qApp, SLOT(quit()));
    }
    else
    {
        cerr << "Listening on: " + ipport + "\n";
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

    cerr << tr("Incoming connection from: ") << socket->peerAddress().toString() << "\n";
    cerr.flush();

    connect(socket, SIGNAL(readyRead()), SLOT(reply()));
    connect(socket, SIGNAL(disconnected()), SLOT(gotDisconnected()));

    Packet::write(socket, OPC_LOGIN);
}

void Server::reply()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    while (socket->canReadLine())
    {
        Packet packet = Packet::read(socket->readLine());

        if (packet.opcode() == OPC_NULL)
            return;

        User * user = user_conns_m.value(socket);

        qDebug() << "Handling data, opcode:" << packet.opcode() << ", data: " << packet.data();

        if (packet.opcode() == OPC_LOGIN || packet.opcode() == OPC_FORCELOGIN)
        {
            if (user)
            {
                Packet::write(socket, OPC_ALREADYLOGGED);
                return;
            }

            QList<QByteArray> args = packet.data().split(' ');

            QString userName, pass;

            if (args.size() == 2)
            {
                userName = args.at(0);
                pass = args.at(1);
            }

            login(socket, userName, pass);
            return;
        }

        if (!user)
        {
            QString ip(socket->peerAddress().toString());

            socket->disconnectFromHost();

            cerr << tr("A client (IP=%1) has requested something without being logged in,").arg(ip)
                 << "\n" << tr("which is suspicious - closing the connection.") << "\n";
            cerr.flush();
        }

        switch (packet.opcode())
        {
            case OPC_REQUEST_CONNECTION:
            {
                QList<QByteArray> parts = packet.data().split('|');
                estabilishIRCconnectionForUser(parts[0], parts[1].split(' '), user);
                break;
            }
            case OPC_IRC_SEND:
            {
                IRCconnection * con = IRC_conns_m.value(user);
                if (con)
                    con->sendCommandAsap(packet.data().trimmed());
                else
                    Packet::write(socket, OPC_ERROR, "No IRC connection to send to!");

                break;
            }
            default:
                Packet::write(socket, OPC_INVALIDCMD);

                cerr << "Received invalid opcode: " << packet.opcode() << IRC::END;
                cerr.flush();

                return;
        }
    }
}

void Server::estabilishIRCconnectionForUser(QByteArray host, QList<QByteArray> channels, User* user)
{
    QList<QByteArray> hostParts = host.split(':');
    QByteArray address  = hostParts[0].trimmed();
    quint16 port = hostParts[1].trimmed().toInt();

    IRCconnection * conn = new IRCconnection(address, port);
    IRC_conns_m.insert(user, conn);
    QString username = user->userName_m;
    conn->connectAs(username, username, username ,username);

    foreach (QByteArray channel, channels)
        conn->sendCommandAsap(MessageBuilder::Join(channel));

    connect(conn, SIGNAL(messageReceived(QByteArray)), SLOT(handleReceivedMessage(QByteArray)));
}

void Server::handleReceivedMessage(QByteArray message)
{
    IRCconnection* connection = qobject_cast<IRCconnection*>(sender());
    User* owner = IRC_conns_m.key(connection);
    QTcpSocket * socket = user_conns_m.key(owner);

    Packet::write(socket, OPC_MESSAGE_RECIEVED, message);
}

void Server::login(QTcpSocket* socket, const QString& user, const QString& pass)
{
    User* desiredUser = sUsers.GetUser(user, false);
    bool loggedOk;
    if (!desiredUser) // not present
    {
        desiredUser = sUsers.GetUser(user, true);
        desiredUser->Set(USER_STRING_PASSWORD, pass);
        loggedOk = true;
    }
    else
        loggedOk = pass == desiredUser->Get(USER_STRING_PASSWORD);

    Opcode opcode;
    QString message;
    if (loggedOk)
    {
        user_conns_m.insert(socket, desiredUser);

        opcode = OPC_LOGGED;
        message = "User " + user + " successfully logged in";
    }
    else
    {
        socket->disconnectFromHost();
        QString ip(socket->peerAddress().toString());
        log.insert(ip, log.value(ip, 0)+1);

        opcode = OPC_WRONGLOGIN;
        message = "User " + user + " tried to log in with a wrong password, closing the connection";
    }

    Packet::write(socket, opcode);
    cerr << message << IRC::END;
    cerr.flush();
}

void Server::gotDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>( sender() );

    User * user = user_conns_m.value(socket);

    QString str;
    if (!user)
        str = "*not logged*";
    else
    {
        user_conns_m.remove(socket);
        str = user->userName_m;
    }

    socket->deleteLater();

    cerr << "Client " + str + " has disconnected" << "\n";
    cerr.flush();
}
