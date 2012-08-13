#include "user.h"
#include <QStringList>
#include "usermgr.h"

static const QString User_IntKeys[] =
{
    "last_logged",          // USER_INT_LAST_LOGGED
};

static const QString User_StringKeys[] =
{
    "password",             // USER_STRING_PASSWORD
};

static const QString User_BoolKeys[] =
{
    "is_logged",            // USER_BOOL_LOGGED
};

quint32 User::Get(User_Int item)
{
    return (quint32)sUsers.GetSettings().value(KeyFullName(User_IntKeys[item])).toInt();
}

QString User::Get(User_String item)
{
    return sUsers.GetSettings().value(KeyFullName(User_StringKeys[item])).toString();
}

bool User::Get(User_Bool item)
{
    return sUsers.GetSettings().value(KeyFullName(User_BoolKeys[item])).toBool();
}

void User::Set(User_Int item, quint32 val)
{
    sUsers.GetSettings().setValue(KeyFullName(User_IntKeys[item]), val);
}

void User::Set(User_String item, const QString &val)
{
    sUsers.GetSettings().setValue(KeyFullName(User_StringKeys[item]), val);
}

void User::Set(User_Bool item, bool val)
{
    sUsers.GetSettings().setValue(KeyFullName(User_BoolKeys[item]), val);
}

QString User::KeyFullName(const QString& partialKey) const
{
    return "l" + userName_m + "/" + partialKey;
}

