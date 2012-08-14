#include "server.h"

#include <QtNetwork>
#include <cstdio>
#include <ircconstants.h>
#include <messagehandler.h>
#include "usermgr.h"
#include <firc.h>
#include <QHash>

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
    UserConnection* userConn = getConnection(qobject_cast<QTcpSocket*>(sender()));

    while (socket->canReadLine())
    {
        Packet packet = Packet::read(socket->readLine());

        if (packet.opcode() == OPC_NULL)
            return;

        qDebug() << "Handling data, opcode:" << packet.opcode() << ", data: " << packet.data();

        if (packet.opcode() == OPC_LOGIN || packet.opcode() == OPC_FORCELOGIN)
        {
            if (userConn)
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

        if (!userConn)
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
                QByteArray connMethod = parts[0];

                IRCconnection * conn = NULL;
                if (connMethod == fIRC::ConnectionMethod::New)
                {
                    QByteArray label = parts[1];
                    QByteArray host = parts[2];
                    QByteArray nick = parts[3];
                    QList<QByteArray> channels = parts[4].split(' ');
                    conn = newIRCconn(userConn->user_m, label, host, nick, channels);
                    userConn->addIRCtoListen(conn);
                }
                else if (connMethod == fIRC::ConnectionMethod::Connect)
                {
                    QByteArray label = parts[1];
                    QByteArray host = parts[2];
                    QByteArray nick = parts[3];
                    conn = getIRCConnection(userConn->user_m, label, host, nick);
                    userConn->addIRCtoListen(conn);
                }
                else
                {
                    Packet::write(userConn->socket_m, OPC_ERROR, "ERROR at opcode " + QByteArray::number(packet.opcode()) + ": unknown connection method \"" + connMethod + "\"");
                    break;
                }

                if (conn)
                    userConn->addIRCtoListen(conn);
                else
                    Packet::write(userConn->socket_m, OPC_ERROR, "ERROR at opcode " + QByteArray::number(packet.opcode()) + ": connection couldn't be estabilished or was not found");

                break;
            }
            case OPC_IRC_SEND:
            {
                // TODO specify server in packet
                IRCconnection * con = userConn->listeningIRCconns_m.first();
                if (con)
                    con->sendCommandAsap(packet.data().trimmed());
                else
                    Packet::write(userConn->socket_m, OPC_ERROR, "No IRC connection to send to!");

                break;
            }
            case OPC_REQ_IRC_LIST:
            {
                Packet::write(userConn->socket_m, OPC_IRC_CONN_DATA, fIRC::Position::Start);

                User* user = userConn->user_m;
                for(QList<QPair<User*, IRCconnection*> >::Iterator itr = IRC_conns_m.begin(); itr != IRC_conns_m.end(); ++itr)
                    if ((*itr).first == user)
                        if (IRCconnection* conn = (*itr).second)
                            Packet::write(userConn->socket_m, OPC_IRC_CONN_DATA, conn->GetLabel().toUtf8() + "|" + conn->Address().toUtf8().append(":").append(QByteArray::number(conn->Port())) + "|" + conn->ownNick().toUtf8());

                Packet::write(userConn->socket_m, OPC_IRC_CONN_DATA, fIRC::Position::End);
                break;
            }
            default:
                Packet::write(userConn->socket_m, OPC_INVALIDCMD);

                cerr << "Received invalid opcode: " << packet.opcode() << IRC::END;
                cerr.flush();

                return;
        }
    }
}

IRCconnection* Server::newIRCconn(User* user, QByteArray label, QByteArray host, QByteArray nick, QList<QByteArray> channels)
{
    QList<QByteArray> hostParts = host.split(':');
    QByteArray address  = hostParts[0].trimmed();
    quint16 port = hostParts[1].trimmed().toInt();

    IRCconnection * conn = new IRCconnection(address, port, label);
    IRC_conns_m.append(qMakePair(user, conn));
    QString nickname = nick.isEmpty() ? user->userName_m : nick;
    conn->connectAs(nickname, user->userName_m, "fIRC", "");

    foreach (QByteArray channel, channels)
        conn->sendCommandAsap(MessageBuilder::Join(channel));

    connect(conn, SIGNAL(messageReceived(QByteArray)), SLOT(handleReceivedMessage(QByteArray)));
    return conn;
}

void Server::handleReceivedMessage(QByteArray message)
{
    IRCconnection* connection = qobject_cast<IRCconnection*>(sender());
    foreach(UserConnection* userConn, connections_m)
        if (userConn->listeningIRCconns_m.contains(connection))
            Packet::write(userConn->socket_m, OPC_MESSAGE_RECIEVED, message);
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
        connections_m.insert(socket, new UserConnection(desiredUser, socket));

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

    UserConnection* userConn = getConnection(socket);

    QString str;
    if (!userConn)
        str = "*not logged*";
    else
    {
        connections_m.remove(socket);
        str = userConn->user_m->userName_m;
    }

    socket->deleteLater();

    cerr << "Client " + str + " has disconnected" << "\n";
    cerr.flush();
}

UserConnection* Server::getConnection(QTcpSocket *socket) const
{
    /*foreach (UserConnection* conn, connections_m)
        if (conn->socket_m == socket)
            return conn;

    return NULL;*/
    return connections_m.value(socket);
}

IRCconnection* Server::getIRCConnection(User* user, QByteArray label, QByteArray host, QByteArray nick) const
{
    for(QList<QPair<User*, IRCconnection*> >::ConstIterator itr = IRC_conns_m.begin(); itr != IRC_conns_m.end(); ++itr)
        if ((*itr).first == user)
            if (IRCconnection* conn = (*itr).second)
                if (conn->GetLabel() == label && conn->HostName() == host && conn->ownNick() == nick)
                    return conn;

    return NULL;
}

