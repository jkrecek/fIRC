#ifndef USERCONNECTION_H
#define USERCONNECTION_H

#include "user.h"
#include <ircconnection.h>

class UserConnection
{
    public:
        UserConnection(User* user, QTcpSocket* socket);

        void addIRCtoListen(IRCconnection* conn);

        User* user_m;
        QTcpSocket* socket_m;
        QList<IRCconnection*> listeningIRCconns_m;
};

#endif // USERCONNECTION_H
