#ifndef USERMGR_H
#define USERMGR_H

#include <QSettings>
#include "user.h"
#include <Singleton.h>

class UserMgr
{
    public:
        UserMgr();

        User* GetUser(QString userName, bool force = false);
        QString settingsValue(QString userName, QString key) const;

        QSettings& GetSettings() { return settings_m; }

    private:
        QSettings settings_m;
        QMap<QString, User*> users_m;
};

#define sUsers Singleton<UserMgr>::Instance()

#endif // USERMGR_H
