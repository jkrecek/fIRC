#ifndef CONNECTIONLIST_H
#define CONNECTIONLIST_H

#include <QDialog>

#include "connectionmanager.h"
#include <QListWidgetItem>

namespace Ui {
    class ConnectionList;
}

class ConnectionList : public QDialog
{
        Q_OBJECT
        
    public:
        explicit ConnectionList(QWidget *parent = 0);
        ~ConnectionList();

    private slots:
        void callConnect();
        void callNew();
        void callEdit();
        void callDelete();

        void reloadConnectionList();
        void itemToConnectTo(QListWidgetItem*);

        ConnectionDetails* getSelectedConnection();

    private:
        void doConnect(ConnectionDetails* details);
        Ui::ConnectionList *ui;
};

#endif // CONNECTIONLIST_H
