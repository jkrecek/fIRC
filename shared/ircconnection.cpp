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

void IRCconnection::sendCommandAsap(const QByteArray& command)
{
    commandsInQueue_m.push_back(command.trimmed());
    if (isLoggedIn())
        sendCommandsInQueue();
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
        Message message = MessageParser::incomingMessage(messageInRawText);

        if (message.isPing())
        {
            send("PONG :" + message.content().toUtf8());
            continue;
        }

        if (message.command() == IRC::Reply::WELCOME)
        {
            loggedIn_m = true;
            sendCommandsInQueue();
        }

        emit messageReceived(messageInRawText);        
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

void IRCconnection::disconnect(const QByteArray& quitMessage)
{
    if (socket_m->isOpen())
    {
        send("QUIT :" + quitMessage);
        socket_m->close();
    }
}
