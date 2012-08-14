#include "ircconnectionselectdialog.h"
#include "ui_ircconnectionselectdialog.h"
#include <QDebug>

#define SELECT_ITEM_ROLE_ID 1
IRCconnectionSelectDialog::IRCconnectionSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IRCconnectionSelectDialog),
    idx(0)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(sItemSelected(QListWidgetItem*)));
    connect(this, SIGNAL(sendSelection(IRCconnectionSelectDialog::ConnSelectItem)), parent, SLOT(connectionSelected(IRCconnectionSelectDialog::ConnSelectItem)));
}

IRCconnectionSelectDialog::~IRCconnectionSelectDialog()
{
    delete ui;
}

void IRCconnectionSelectDialog::sItemSelected(QListWidgetItem *l)
{
    if (quint16 idx = l->data(SELECT_ITEM_ROLE_ID).toInt())
        emit sendSelection(items.value(idx));
    else
        emit sendSelection(ConnSelectItem());
}

void IRCconnectionSelectDialog::addItem(ConnSelectItem item)
{
    quint16 thisIdx = ++idx;
    QListWidgetItem* widgetItem = new QListWidgetItem();
    widgetItem->setText(item.label + "(host: " + item.host + ", nick: " + item.nick + ")");
    widgetItem->setData(SELECT_ITEM_ROLE_ID, thisIdx);
    items.insert(thisIdx, item);
    ui->listWidget->addItem(widgetItem);
}
