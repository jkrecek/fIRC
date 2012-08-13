#ifndef IRCCONNECTIONSELECTDIALOG_H
#define IRCCONNECTIONSELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui
{
    class IRCconnectionSelectDialog;
}

class IRCconnectionSelectDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit IRCconnectionSelectDialog(QWidget *parent);
        ~IRCconnectionSelectDialog();

    signals:
        void sendSelection(QString str);

    private slots:
        void sItemSelected(QListWidgetItem* l);

    private:
        Ui::IRCconnectionSelectDialog *ui;
};

#endif // IRCCONNECTIONSELECTDIALOG_H
