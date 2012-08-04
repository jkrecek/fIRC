#include <QList>
#include <QTextCodec>
#include "IRCconnection.h"
#include "ircconstants.h"
#include "messageparser.h"

IRCconnection::IRCconnection(const QString& serverAddress, quint16 serverPort, QObject* parent) :
    QObject(parent),
    serverAddress_m(serverAddress),
    serverPort_m(serverPort),
    socket_m(0),
    loggedIn_m(false),
    codecName_m("UTF-8"),
    codec_m(QTextCodec::codecForName(codecName_m))
{

}

IRCconnection::~IRCconnection()
{
    disconnect();
    socket_m->deleteLater();
}

void IRCconnection::connectAs(const QString& nickName, const QString& userName, const QString& realName, const QString& userPassword)
{
    nickName_m = nickName;
    userName_m = userName;
    realName_m = realName;
    userPassword_m = userPassword;

    connectToServer();
}

void IRCconnection::connectToServer()
{
    socket_m = new QTcpSocket(this);

    connect(socket_m, SIGNAL(connected()), this, SLOT(login()));
    connect(socket_m, SIGNAL(disconnected()), this, SLOT(disconnect()));
    connect(socket_m, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured(QAbstractSocket::SocketError)));
    connect(socket_m, SIGNAL(readyRead()), this, SLOT(readData()));

    socket_m->connectToHost(serverAddress_m, serverPort_m);
}

void IRCconnection::login()
{
    QByteArray data;
    data.append(IRC::PassIdentifyPrefix + " " + userPassword_m + "\n");
    data.append(IRC::NickIdentifyPrefix + " " + nickName_m + "\n");
    data.append(IRC::UserIdentifyPrefix + " " + userName_m + " cotoje tady :" + realName_m);
    send(data);
}

void IRCconnection::joinChannel(const QString& channelName)
{
    addJoinChannelRequestToQueue(channelName);
}

void IRCconnection::addJoinChannelRequestToQueue(const QString& channelName)
{
    QByteArray command = QByteArray("JOIN " + channelName.toUtf8());
    sendCommandAsap(command);
}

void IRCconnection::changeNickName(const QString& nickName)
{
    addChangeNickNameRequestToQueue(nickName);
}

void IRCconnection::addChangeNickNameRequestToQueue(const QString& nickName)
{
    QByteArray command = QByteArray("NICK " + nickName.toUtf8());
    sendCommandAsap(command);
}

void IRCconnection::sendActionToChannel(const QString& channelName, const QString& actionContent)
{
    addChannelActionToQueue(channelName, actionContent);
}

void IRCconnection::addChannelActionToQueue(const QString& channelName, const QString& actionContent)
{
    QByteArray command = "PRIVMSG " + channelName.toUtf8() + " :" + char(1) + "ACTION " + actionContent.toUtf8() + char(1);
    sendCommandAsap(command);
}

void IRCconnection::sendActionToUser(const QString& userName, const QString& actionContent)
{
    addChannelMessageToQueue(userName, actionContent);
}

void IRCconnection::addPrivateActionToQueue(const QString& userName, const QString& actionContent)
{
    QByteArray command = "PRIVMSG " + userName.toUtf8() + " :" + char(1) + "ACTION " + actionContent.toUtf8() + char(1);
    sendCommandAsap(command);
}

void IRCconnection::sendMessageToChannel(const QString& channelName, const QString& messageContent)
{
    addChannelMessageToQueue(channelName, messageContent);
}

void IRCconnection::addChannelMessageToQueue(const QString& channelName, const QString& messageContent)
{
    QByteArray begin = "PRIVMSG " + channelName.toUtf8() + " :";
    int allowedSize = MESSAGE_MAX - begin.size();

    QByteArray next = messageContent.toUtf8();
    while(!next.isNull())
    {
        if (next.size() <= allowedSize)
        {
            sendCommandAsap(begin+next.trimmed());
            break;
        }

        int idx = next.lastIndexOf(" ", allowedSize);
        sendCommandAsap(begin+next.left(idx).trimmed());
        next = next.mid(idx);
    }
}

void IRCconnection::sendMessageToUser(const QString& userName, const QString& messageContent)
{
    addPrivateMessageToQueue(userName, messageContent);
}

void IRCconnection::sendNoticeToUser(const QString &userName, const QString &messageContent)
{
    QByteArray command = "NOTICE " + userName.toUtf8() + " :" + messageContent.toUtf8();
    sendCommandAsap(command);
}

void IRCconnection::addPrivateMessageToQueue(const QString& userName, const QString& messageContent)
{
    QByteArray begin = "PRIVMSG " + userName.toUtf8() + " :";
    int allowedSize = MESSAGE_MAX - begin.size();

    QByteArray next = messageContent.toUtf8();
    while(!next.isNull())
    {
        if (next.size() <= allowedSize)
        {
            sendCommandAsap(begin+next.trimmed());
            break;
        }

        int idx = next.lastIndexOf(" ", allowedSize);
        sendCommandAsap(begin+next.left(idx).trimmed());
        next = next.mid(idx);
    }
}

void IRCconnection::sendCommandAsap(const QByteArray& command)
{
    if (isLoggedIn())
        send(command.trimmed());
    else
    {
        commandsInQueue_m.push_back(command.trimmed());
        enqueuedCommand();
    }
}

void IRCconnection::sendCommandsInQueue()
{
    if (isLoggedIn())
    {
        while(!commandsInQueue_m.isEmpty())
        {
            QByteArray command = commandsInQueue_m.takeFirst();
            send(command);
        }
    }
}

void IRCconnection::enqueuedCommand()
{
    if (loggedIn_m)
        sendCommandsInQueue();
}

void IRCconnection::sendMessage(const QString& rawText)
{
    QByteArray rawData = rawText.toUtf8();
    send(rawData);
}

void IRCconnection::send(const QByteArray& rawData)
{
    socket_m->write(rawData + "\n");
    socket_m->flush();
;
}

void IRCconnection::errorOccured(QAbstractSocket::SocketError)
{
    // TODO
}

void IRCconnection::readData()
{
    while (socket_m->canReadLine())
    {
        QByteArray messageInRawText = socket_m->readLine();
        emit messageReceived(messageInRawText);

        Message message = MessageParser::receivedRawTextToMessage(messageInRawText);
        if (message.isServerMessage())
            handleServerMessage(message);
    }
}

void IRCconnection::handleServerMessage(const Message& message)
{
    if (message.isPing())
        sendMessage("PONG :" + message.content());
    else if (message.command() == IRC::Reply::WELCOME)
    {
        loggedIn_m = true;
        sendCommandsInQueue();
    }
}

QByteArray IRCconnection::encode(const QString& text)
{
    QTextEncoder* encoder = codec_m->makeEncoder();
    return encoder->fromUnicode(text);
}

void IRCconnection::setEncoding(const QByteArray& codecName)
{
    if (codecName != codecName_m)
    {
        codecName_m = codecName;
        codec_m = QTextCodec::codecForName(codecName_m);
    }
}

bool IRCconnection::isLoggedIn() const
{
    return loggedIn_m;
}

void IRCconnection::disconnect(const QByteArray& quitMessage)
{
    if (socket_m->isOpen())
    {
        send("QUIT :" + quitMessage);
        socket_m->close();
    }
}
