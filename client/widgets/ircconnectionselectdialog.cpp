#include "ircconnectionselectdialog.h"
#include "ui_ircconnectionselectdialog.h"
#include <QDebug>

IRCconnectionSelectDialog::IRCconnectionSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IRCconnectionSelectDialog)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(sItemSelected(QListWidgetItem*)));
    connect(this, SIGNAL(sendSelection(QString)), parent, SLOT(connectionSelected(QString)));
}

IRCconnectionSelectDialog::~IRCconnectionSelectDialog()
{
    delete ui;
}

void IRCconnectionSelectDialog::sItemSelected(QListWidgetItem *l)
{
    QString selection = l->text();
    done(1);
    emit sendSelection(selection);
}
