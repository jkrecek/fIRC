#include "userconnection.h"

UserConnection::UserConnection(User *user, QTcpSocket *socket) :
    user_m(user),
    socket_m(socket)
{

}

void UserConnection::addIRCtoListen(IRCconnection* conn)
{
    listeningIRCconns_m.push_back(conn);
}
