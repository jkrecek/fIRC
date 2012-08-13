#ifndef USER_H
#define USER_H

#include <QString>
#include <QSettings>

enum User_Int
{
    USER_INT_LAST_LOGGED = 0,
    USER_INT_COUNT
};

enum User_String
{
    USER_STRING_PASSWORD = 0,
    USER_STRING_COUNT
};

enum User_Bool
{
    USER_BOOL_LOGGED = 0,
    USER_BOOL_COUNT
};


class User
{
    public:
        User(const QString& userName) : userName_m(userName) { }

        quint32 Get(User_Int item);
        QString Get(User_String item);
        bool Get(User_Bool item);

        void Set(User_Int item, quint32 val);
        void Set(User_String item, const QString& val);
        void Set(User_Bool item, bool val);

        QString KeyFullName(const QString& partialKey) const;

        QString userName_m;
};

#endif // USER_H
