#ifndef FIRCWINDOW_H
#define FIRCWINDOW_H

#include <QTextBrowser>
#include "channeluser.h"

namespace Ui {
    class fIRCtab;
}

class fIRCtab : public QTextBrowser
{
    Q_OBJECT

    public:
        fIRCtab(QString tabName);
        ~fIRCtab();

        void setUserList(QList<ChannelUser> userList) { userList_m = userList; }

    private:
        QString tabName_m;
        QList<ChannelUser> userList_m;
        Ui::fIRCtab *ui;
};

#endif // FIRCWINDOW_H
