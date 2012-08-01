#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QByteArray>

#include <socket.h>

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

    private:
        Ui::MainWindow *ui;
        QTcpSocket* socket;
        QLabel* statusLabel;

        void setStatus(const QString& statusmsg);

        void login(bool force = false);
        void lockGui(bool lock = true);

    private slots:
        void doConnect();
        void doSend();
        void gotConnected();
        void gotDisconnected();
        void gotError(QAbstractSocket::SocketError error);
        void handleReply();
};

#endif // MAINWINDOW_H
