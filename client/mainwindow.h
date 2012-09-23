#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QByteArray>
#include <QSettings>
#include <QTextBrowser>

#include <packet.h>
#include <message.h>

#include "widgets/ircconnectionselectdialog.h"
#include "widgets/ircconnectioncreatedialog.h"
#include "connectdialog.h"

class MyTcpSocket;
class QTcpSocket;
class QLabel;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);

    private slots:
        void doConnect();
        void doSend();
        void gotConnected();
        void gotDisconnected();
        void gotError(QAbstractSocket::SocketError);
        void handleReply();
        void connectionSelected(IRCconnectionSelectDialog::ConnSelectItem);
        void connectionDataSet(IRCconnectionCreateDialog::ConnCreateItem);
        void connectionCreated();

    private:
        void setStatus(const QString& statusmsg);
        void sendConnect(QByteArray connectionMethod, QByteArray label, QByteArray host, QByteArray nick, QStringList channels = QStringList());

        void login(bool force = false);
        void lockGui(bool lock = true);

        void showMessage(const Message message);
        QTextBrowser* createTab(QString channelName);

        Ui::MainWindow *ui;
        QTcpSocket* socket;
        QLabel* statusLabel;
        QMap<QString, quint16> indexMap;

        QSettings settings;

        QString pendingChannel;
        QDialog* pendingDialog;

        QList<QByteArray> dataBuffer;
};

#endif // MAINWINDOW_H
