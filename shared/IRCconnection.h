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
        IRCconnection(const QString& serverAdress, quint16 serverPort = 6667, QObject *parent = 0);
        ~IRCconnection();
        
        bool isLoggedIn() const;
        QString ownNick() const { return nickName_m; }

    public slots:
        void connectAs(const QString& nickName, const QString& userName, const QString& realName, const QString& userPassword);
        void changeNickName(const QString& nickName);
        void sendMessageToChannel(const QString& channelName, const QString& messageContent);
        void sendMessageToUser(const QString& userName, const QString& messageContent);
        void sendNoticeToUser(const QString& userName, const QString& messageContent);
        void sendActionToChannel(const QString& channelName, const QString& messageContent);
        void sendActionToUser(const QString& userName, const QString& messageContent);
        void sendCommandAsap(const QByteArray& command);
        void joinChannel(const QString& channelName);
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
        void sendMessage(const QString& rawText);

        void addChannelMessageToQueue(const QString& channelName, const QString& messageContent);
        void addPrivateMessageToQueue(const QString& userName, const QString& messageContent);
        void addChannelActionToQueue(const QString& channelName, const QString& messageContent);
        void addPrivateActionToQueue(const QString& userName, const QString& messageContent);
        void addJoinChannelRequestToQueue(const QString& channelName);
        void addChangeNickNameRequestToQueue(const QString& nickName);
        void handleServerMessage(const Message& message);

        void send(const QByteArray& rawData);

        void sendCommandsInQueue();
        void enqueuedCommand();

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

        QList<QByteArray> commandsInQueue_m;
};

#endif // IRCCONNECTION_H
