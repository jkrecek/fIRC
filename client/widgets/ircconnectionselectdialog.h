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
        struct ConnSelectItem
        {
            public:
                ConnSelectItem() : valid(false) { }
                QByteArray label;
                QByteArray host;
                QByteArray nick;
                bool valid;
        };

        explicit IRCconnectionSelectDialog(QWidget *parent);
        ~IRCconnectionSelectDialog();

        void addItem(ConnSelectItem item);
        bool isEmpty() const { return items.isEmpty(); }

    signals:
        void sendSelection(IRCconnectionSelectDialog::ConnSelectItem);

    private slots:
        void sItemSelected(QListWidgetItem* l);

    private:
        Ui::IRCconnectionSelectDialog *ui;

        quint16 idx;
        QHash<quint16, ConnSelectItem> items;
};

#endif // IRCCONNECTIONSELECTDIALOG_H
