#ifndef FIRCWINDOW_H
#define FIRCWINDOW_H

#include <QTextBrowser>
#include "channeluser.h"

class fIRCtab : public QTextBrowser
{
    public:
        fIRCtab(QString tabName);

        void setUserList(QList<ChannelUser> userList) { userList_m = userList; }

    private:
        QString tabName_m;
        QList<ChannelUser> userList_m;
};

#endif // FIRCWINDOW_H
