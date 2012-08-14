#ifndef IRCCONNECTIONCREATEDIALOG_H
#define IRCCONNECTIONCREATEDIALOG_H

#include <QDialog>

namespace Ui {
    class IRCconnectionCreateDialog;
}

class IRCconnectionCreateDialog : public QDialog
{
        Q_OBJECT
        
    public:
        struct ConnCreateItem
        {
            public:
                QByteArray label;
                QByteArray host;
                QByteArray nick;
                QStringList channels;
        };

        explicit IRCconnectionCreateDialog(QWidget *parent = 0);
        ~IRCconnectionCreateDialog();

        void accept();

    signals:
        void sendCreateConnData(IRCconnectionCreateDialog::ConnCreateItem);

    private:
        Ui::IRCconnectionCreateDialog *ui;
};

#endif // IRCCONNECTIONCREATEDIALOG_H
