#ifndef IRCCONNECTION_H
#define IRCCONNECTION_H

#include <QObject>
#include <QMap>
#include <QTcpSocket>
#include "message.h"

#define MESSAGE_MAX 471

class IRCconnection : public QObject
{
    Q_OBJECT

    public:
        IRCconnection(const QString& serverAddress, quint16 serverPort = 6667, const QString& label = "", QObject *parent = 0);
        ~IRCconnection();
        
        bool isLoggedIn() const { return loggedIn_m; }
        QString ownNick() const { return nickName_m; }

        QString GetLabel() const { return label_m; }
        QString Address() const { return serverAddress_m; }
        quint16 Port() const { return serverPort_m; }
        QString HostName() const { return Address() + ":" + QString::number(Port()); }

    public slots:
        void connectAs(const QString& nickName, const QString& userName, const QString& realName, const QString& userPassword);
        void sendCommandAsap(const QByteArray& command);
        void setEncoding(const QByteArray& codecName);
        void connectToServer();
        void disconnect(const QByteArray& quitMessage = QByteArray("Disconnected for unknown reason"));

    signals:
        void messageReceived(QByteArray message);

    private slots:
        void errorOccured(QAbstractSocket::SocketError);
        void readData();
        void login();

    private:
        void send(const QByteArray& rawData);

        void sendCommandsInQueue();

        QByteArray encode(const QString& text);

        QString serverAddress_m;
        quint16 serverPort_m;
        QTcpSocket* socket_m;

        QString nickName_m;
        QString userName_m;
        QString realName_m;
        QString userPassword_m;
        bool loggedIn_m;

        QByteArray codecName_m;
        QTextCodec* codec_m;

        QString label_m;

        QList<QByteArray> commandsInQueue_m;
};

#endif // IRCCONNECTION_H
