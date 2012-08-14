#include "ircconnectioncreatedialog.h"
#include "ui_ircconnectioncreatedialog.h"
#include <QDebug>

IRCconnectionCreateDialog::IRCconnectionCreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IRCconnectionCreateDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendCreateConnData(IRCconnectionCreateDialog::ConnCreateItem)), parent, SLOT(connectionDataSet(IRCconnectionCreateDialog::ConnCreateItem)));
    //connect(ui->buttonBox, SIGNAL(accepted()))
}

IRCconnectionCreateDialog::~IRCconnectionCreateDialog()
{
    delete ui;
}

void IRCconnectionCreateDialog::accept()
{
    ConnCreateItem item;
    item.label = ui->editLabel->text().toUtf8();
    item.host = ui->editAddress->text().toUtf8() + ":" + ui->portEdit->text().toUtf8();
    item.nick = ui->editNick->text().toUtf8();
    item.channels = ui->editChannels->text().split(" ");

    emit sendCreateConnData(item);

    QDialog::accept();
}
