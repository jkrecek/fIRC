#include "connectionlist.h"
#include "ui_connectionlist.h"

#include "connectdialog.h"
#include <QMessageBox>

#include "firc.h"
#include <QPointer>
#include <QDebug>

#define DATA_PART 4

ConnectionList::ConnectionList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionList)
{
    ui->setupUi(this);

    //ui->buttonBox->addButton(ui->pushButton, QDialogButtonBox::ActionRole);
    connect(ui->buttonConnect, SIGNAL(clicked()), SLOT(callConnect()));
    connect(ui->buttonNew, SIGNAL(clicked()), SLOT(callNew()));
    connect(ui->buttonEdit, SIGNAL(clicked()), SLOT(callEdit()));
    connect(ui->buttonDelete, SIGNAL(clicked()), SLOT(callDelete()));
    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(reject()));

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(itemToConnectTo(QListWidgetItem*)));

    reloadConnectionList();
}

ConnectionList::~ConnectionList()
{
    delete ui;
}

void ConnectionList::callConnect()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if (items.isEmpty())
    {
        QMessageBox::critical(this, "Nothing selected", "You must select atleast one connection to connect to");
        return;
    }

    if (ConnectionDetails* connection = getSelectedConnection())
        doConnect(connection);
}

void ConnectionList::callNew()
{
    (new ConnectDialog(this))->show();
}

void ConnectionList::callEdit()
{

}

void ConnectionList::callDelete()
{
    ConnectionDetails* connection = getSelectedConnection();
    if (!connection)
        return;

    QMessageBox::StandardButton r = QMessageBox::warning(this,
        "Delete confirmation",
        "Are you sure you want to delete this connection?",
        QMessageBox::Yes|QMessageBox::No, QMessageBox::No);

    if (r == QMessageBox::Yes)
    {
        sConnections.deleteConnection(connection, true);
        reloadConnectionList();
    }
}

void ConnectionList::reloadConnectionList()
{
    ui->listWidget->clear();
    foreach (ConnectionDetails* details, sConnections.getConnections())
    {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setData(DATA_PART, details->m_label);

        QString line = details->m_label;
        line += " (";
        if (details->m_method == CON_LOCAL)
        {
            line += fIRC::Connection::ConnectionMethod::Local + ", " + static_cast<LocalConnectionDetails*>(details)->m_address;
            item->setTextColor("green");
        }
        else
        {
            line += fIRC::Connection::ConnectionMethod::Remote + ", " + static_cast<RemoteConnectionDetails*>(details)->m_address;
            item->setTextColor("red");
        }

        line += ")";
        item->setText(line);
    }
}

void ConnectionList::itemToConnectTo(QListWidgetItem* item)
{
    QString _connLabel = item->data(DATA_PART).toString();
    ConnectionDetails* details = sConnections.getConnection(_connLabel);
    doConnect(details);
}

void ConnectionList::doConnect(ConnectionDetails *details)
{
    if (details == NULL)
    {
        qDebug() << "null";
        return;
    }

    qDebug() << "connecting to " + details->m_label;
}

ConnectionDetails* ConnectionList::getSelectedConnection()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if (items.isEmpty())
    {
        QMessageBox::critical(this, "Nothing selected", "You must select atleast one connection to connect to");
        return NULL;
    }

    QListWidgetItem* selected = items.first();
    QString connLabel = selected->data(DATA_PART).toString();
    return sConnections.getConnection(connLabel);

}
