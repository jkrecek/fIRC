#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtNetwork>
#include <ircconstants.h>

#include <messagebuilder.h>
#include <firc.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationDomain(), QCoreApplication::applicationName())
{
	ui->setupUi(this);

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), SLOT(gotConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(gotDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(gotError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), SLOT(handleReply()));

    statusLabel = new QLabel();
    statusBar()->addWidget(statusLabel);

    connect(ui->connBtn, SIGNAL(clicked()), SLOT(doConnect()));
    connect(ui->passEdit, SIGNAL(returnPressed()), SLOT(doConnect()));
    connect(ui->sendBtn, SIGNAL(clicked()), SLOT(doSend()));
    connect(ui->todoEdit, SIGNAL(returnPressed()), SLOT(doSend()));
    connect(ui->actionConnect, SIGNAL(triggered()), SLOT(doConnect()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), socket, SLOT(disconnectFromHostImplementation()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    lockGui();

    resize(600, 500);
}

void MainWindow::lockGui(bool lock)
{
	ui->actionDisconnect->setDisabled(lock);
    ui->sendBtn->setDisabled(lock);
}

void MainWindow::doConnect()
{
    if (ui->userEdit->text().isEmpty() || ui->userEdit->text().contains(QChar(' ')))
    {
        QMessageBox::critical(this, tr("Invalid username"), tr("The username cannot contain any whitespaces!"));
        return;
    }

    if (ui->passEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Please enter password"), tr("You must enter password to proceed!"));
        return;
    }

	ui->connBtn->setDisabled(true);
	ui->actionConnect->setDisabled(true);
    setStatus(tr("Connecting..."));

	QString strAddr = ui->addressEdit->text();
	int port = ui->portEdit->value();

	QHostAddress addr;

    if (!addr.setAddress(strAddr))
	{
        QMessageBox::critical(this, tr("Invalid IP address"), tr("You've entered an invalid IP address!"));
		ui->addressEdit->setFocus();
		return;
	}

	socket->connectToHost(addr, port);
}

void MainWindow::setStatus(const QString& statusmsg)
{
    QString status = QDate::currentDate().toString(Qt::SystemLocaleShortDate) + " " + QTime::currentTime().toString() + " - " + statusmsg;
	statusLabel->setText(status);
}

void MainWindow::gotConnected()
{
    setStatus(tr("Successfully connected!"));

	ui->actionDisconnect->setEnabled(true);
    lockGui(false);
}

void MainWindow::doSend()
{
    QString message = ui->todoEdit->text();
    if (message.isEmpty())
        return;

    QString channel = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    QByteArray data;
    if (message.startsWith("/"))
    {
        message.remove(0,1);
        int idx = message.indexOf(' ');
        QString command = message.left(idx);
        QString parameters = message.mid(idx+1);
        if (command.toLower() == "me")
            data = MessageBuilder::Action(channel, parameters);
        else
            data = command.toUpper().toUtf8() + " " + parameters.toUtf8();
    }
    else
        data = MessageBuilder::Message(channel, message);


    Message msg = MessageParser::outgoingMessage(data);
    showMessage(msg);

    Packet::write(socket, OPC_IRC_SEND, data);
    ui->todoEdit->clear();
}

void MainWindow::showMessage(const Message message)
{
    QString line;
    QTextBrowser * channelTab = createTab(message.senderChannel());
    /*int newIdx;
    if (indexMap.contains(message.senderChannel()))
    {
        newIdx = indexMap.value(message.senderChannel());
        channelTab = (QTextBrowser*)ui->tabWidget->widget(newIdx);
    }
    else
    {
        newIdx = ui->tabWidget->addTab(new QTextBrowser, message.senderChannel());
        indexMap.insert(message.senderChannel(), newIdx);
        channelTab = (QTextBrowser*)ui->tabWidget->widget(newIdx);
        channelTab->setOpenExternalLinks(true);        
    }

    if (ui->tabWidget->currentIndex() == 0 && newIdx != 0)
        ui->tabWidget->setCurrentIndex(newIdx);*/



    if (message.command() == IRC::Command::Join && message.senderNick() == ui->userEdit->text())
    {
        pendingChannel = message.content();
        createTab(pendingChannel);
        return;
    }
    else if (message.command() == IRC::Reply::TOPIC)
    {
        line = "Topic: '" + message.content() + "'";
    }
    else if (message.command() == IRC::Command::PrivMsg)
    {
        line += "(" + QTime::currentTime().toString() +") ";
        if (!message.senderNick().isEmpty())
            line += "[" + message.senderNick() + "] ";
        line += message.content();
    }
    else
    {
        line += "<" + message.command() + "> ";
        line += "(" + QTime::currentTime().toString() +") ";
        if (!message.senderNick().isEmpty())
            line += "[" + message.senderNick() + "] ";
        line += message.content();
    }
    /*{
        newIdx = ui->tabWidget->addTab(new QTextBrowser, message.content());
        indexMap.insert(message.content(), newIdx);
        channelTab = (QTextBrowser*)ui->tabWidget->widget(newIdx);
        channelTab->setOpenExternalLinks(true);
    }*/

    /*QString content = message.content();

    QString address;

    static QStringList prefixes = (QStringList() << "http://" << "https://" << "www." );
    int idx = 0, lenght;
    foreach (QString prefix, prefixes)
    {
        idx = 0;
        while ((idx = content.toLower().indexOf(prefix, idx)) != -1)
        {
            lenght = content.indexOf(' ', idx) - idx;
            address = content.mid(idx, lenght);
            content.replace(idx, lenght, "<a href=\"" + address + "\">" + address + "</a>");
        }
    }*/

    /*while ((idx = content.toLower().indexOf(QRegExp("https?:"), idx)) != -1)
    {
        lenght = content.indexOf(' ', idx) - idx;
        address = content.mid(idx, lenght);
        content.replace(idx, lenght, "<a href=\"" + address + "\">" + address + "</a>");
    }*/

    //line += "<a href=\"http://www.seznam.cz\">" +  message.content() + "</a>";


    if (!line.isEmpty())
    {
        channelTab->append(line);

        // scroll to bottom of tab
        QScrollBar * bar = channelTab->verticalScrollBar();
        bar->setValue(bar->maximum());
    }
}

QTextBrowser* MainWindow::createTab(QString channelName)
{
    if (indexMap.contains(channelName))
        return (QTextBrowser*)ui->tabWidget->widget(indexMap.value(channelName));
    else
    {
        int idx = ui->tabWidget->addTab(new QTextBrowser, channelName);
        indexMap.insert(channelName, idx);
        QTextBrowser* channelTab = (QTextBrowser*)ui->tabWidget->widget(idx);
        channelTab->setOpenExternalLinks(true);
        ui->tabWidget->setCurrentIndex(idx);
        return channelTab;
    }
}

void MainWindow::gotDisconnected()
{
    setStatus(tr("Disconnected!"));

    //ui->textBrowser->clear();
    lockGui();
	ui->connBtn->setEnabled(true);
	ui->actionDisconnect->setDisabled(true);
	ui->actionConnect->setEnabled(true);
}

void MainWindow::gotError(QAbstractSocket::SocketError /*error*/)
{
    gotDisconnected();
    setStatus(socket->errorString());

	QMessageBox::critical(this, tr("Network error"), socket->errorString());
}

void MainWindow::handleReply()
{
    while (socket->canReadLine())
    {
        Packet packet = Packet::read(socket->readLine());

        if (packet.opcode() == OPC_NULL)
            return;

        qDebug() << "handling data, opcode:" << packet.opcode() << ", data: " << packet.data();

        switch (packet.opcode())
        {
            case OPC_LOGIN:
                login();
                break;
            case OPC_LOGGED:
                setStatus(tr("Logged in..."));
                // request connection
                Packet::write(socket, OPC_REQUEST_CONNECTION, "irc.rizon.net:6667|#soulwell #soulwell2");
                break;
            case OPC_ALREADYLOGGED:
            {
                QMessageBox::StandardButton r = QMessageBox::warning(this, tr("User already logged in!"),
                                 tr("Your account is already logged in, probably from somewhere else. Do you want to force the login and disconnect the other client?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No);

                if (r == QMessageBox::Yes)
                    login(true);
                else
                    socket->disconnectFromHost();

                break;
            }
            case OPC_WRONGLOGIN:
                socket->disconnect(SIGNAL(error(QAbstractSocket::SocketError)));
                QMessageBox::critical(this,
                              tr("Wrong login"),
                              tr("You've entered a wrong username or password!") + "\n" +
                              tr("Connection has been closed by the remote server."));

                connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(gotError(QAbstractSocket::SocketError)));
                break;
            case OPC_MESSAGE_RECIEVED:
            {
                Message msg = MessageParser::incomingMessage(packet.data());
                if (fIRC::isFromGlobalChannel(msg.senderChannel()))
                    msg.setChannel("Global");

                showMessage(msg);
                break;
            }
            default:
                qDebug() << "ERROR, opcode:" << packet.opcode() << ", data: " << packet.data();
                QMessageBox::critical(this,
                        tr("Unknown command from the server"),
                        tr("Server has sent an unknown command, which could mean that your client version is outdated."));
        }
    }
}

void MainWindow::login(bool force)
{
	QString user = ui->userEdit->text();
    if (user.isEmpty() || user.contains(QChar(' ')))
	{
        socket->disconnectFromHost();
        QMessageBox::critical(this, tr("Invalid username"), tr("The username cannot contain any whitespaces!"));
		return;
	}

    if (ui->passEdit->text().isEmpty())
    {
        socket->disconnectFromHost();
        QMessageBox::critical(this, tr("Please enter password"), tr("You must enter password to proceed!"));
        return;
    }

    QString pass = QCryptographicHash::hash(ui->passEdit->text().toAscii(), QCryptographicHash::Sha1).toHex();

    setStatus(tr("Logging in..."));    

    QByteArray data = user.toUtf8() + " " + pass.toUtf8();

    Packet::write(socket, force ? OPC_FORCELOGIN : OPC_LOGIN, data);
}

