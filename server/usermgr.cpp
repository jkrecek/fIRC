#include "usermgr.h"

#include <QCoreApplication>

UserMgr::UserMgr() :
    settings_m(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationDomain(), QCoreApplication::applicationName())
{

}

User* UserMgr::GetUser(QString userName, bool force)
{
    if (!users_m.contains(userName))
    {
        if (force)
            users_m.insert(userName, new User(userName));
        else
            return NULL;
    }

    return users_m.value(userName);
}

QString UserMgr::settingsValue(QString userName, QString key) const
{
    return settings_m.value("l" + userName +"/"+key).toString();
}

