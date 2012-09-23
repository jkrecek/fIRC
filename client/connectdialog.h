#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>

#include "connectionmanager.h"

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog
{
        Q_OBJECT

    public:
        ConnectDialog(QWidget *parent = 0);
        ~ConnectDialog();

        void accept();

    signals:
        void done();

    private slots:
        void changedConnectionMethodLocal(bool);
        void changedConnectionMethodRemote(bool);

    private:
        void setLayoutVisibility(QLayout* layout, bool visibility);
        quint8 getVisibleCount(QLayout* layout);
        void setProperWindowHeight();
        Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
