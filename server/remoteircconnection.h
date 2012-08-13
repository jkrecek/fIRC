#ifndef REMOTEIRCCONNECTION_H
#define REMOTEIRCCONNECTION_H

#include <ircconnection.h>
#include "user.h"

class RemoteIRCconnection : public IRCconnection
{
        Q_OBJECT

    public:
        RemoteIRCconnection(User *owner, const QString &serverAdress, quint16 serverPort, QObject *parent = 0) :
            IRCconnection(serverAdress, serverPort, parent), owner_m(owner) {}
        User* GetOwner() const { return owner_m; }

    private:
        User* owner_m;
};

#endif // REMOTEIRCCONNECTION_H
