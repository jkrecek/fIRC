#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <QDebug>
#include <QPushButton>
#include <firc.h>
#include <QMessageBox>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    changedConnectionMethodLocal(false);
    changedConnectionMethodRemote(false);

    connect(ui->radioLocal, SIGNAL(toggled(bool)), SLOT(changedConnectionMethodLocal(bool)));
    connect(ui->radioRemote, SIGNAL(toggled(bool)), SLOT(changedConnectionMethodRemote(bool)));

    connect(this, SIGNAL(done()), parent, SLOT(reloadConnectionList()));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::changedConnectionMethodLocal(bool toggled)
{
    ui->widgetAddress->setVisible(toggled);
    ui->locWidgetChannels->setVisible(toggled);
    ui->locWidgetNick->setVisible(toggled);

    ui->buttonBox->button(ui->buttonBox->Ok)->setEnabled(toggled);

    if (toggled)
    {
        ui->editAddress->setProperty("placeholderText", "(required, IRC server address)");
        if (ui->editPort->value() == fIRC::Ports::DefaultRemote)
            ui->editPort->setValue(fIRC::Ports::DefaultIRC);
    }

    setProperWindowHeight();
}

void ConnectDialog::changedConnectionMethodRemote(bool toggled)
{
    ui->widgetAddress->setVisible(toggled);
    ui->remWidgetUser->setVisible(toggled);
    ui->remWidgetPass->setVisible(toggled);

    ui->buttonBox->button(ui->buttonBox->Ok)->setEnabled(toggled);

    if (toggled)
    {
        ui->editAddress->setProperty("placeholderText", "(required, remote server address)");
        if (ui->editPort->value() == fIRC::Ports::DefaultIRC)
            ui->editPort->setValue(fIRC::Ports::DefaultRemote);
    }

    setProperWindowHeight();
}

void ConnectDialog::setLayoutVisibility(QLayout *layout, bool visibility)
{
    for (int i = 0; i < layout->count(); ++i)
    {
        if (QWidget* widget = layout->itemAt(i)->widget())
            widget->setVisible(visibility);
        else if (QLayout * lay = layout->itemAt(i)->layout())
            setLayoutVisibility(lay, visibility);
    }
}

void ConnectDialog::setProperWindowHeight()
{
    quint8 visible = getVisibleCount(ui->itemLayout);
    quint16 properSize = 100 + visible * 50;
    ui->itemBox->resize(ui->itemBox->width(), properSize);
    resize(width(), properSize + 50);
}

uint8_t ConnectDialog::getVisibleCount(QLayout *layout)
{
    quint8 count = 0;
    for (int i = 0; i < layout->count(); ++i)
        if (QWidget* widget = layout->itemAt(i)->widget())
            if (widget->isVisible())
                ++count;

    return count;
}

void ConnectDialog::accept()
{
    // already contains label
    QString label = ui->editLabel->text();
    if (label.isEmpty())
    {
        QMessageBox::critical(this, "Invalid label", "Connection must have label!");
        return;
    }
    if (sConnections.hasConnection(label))
    {
        QMessageBox::critical(this, "Invalid label", "You already have connection with such label!");
        return;
    }

    QString address = ui->editAddress->text().trimmed();
    if (address.isEmpty())
    {
        QMessageBox::critical(this, "Invalid address", "You must enter address to connect to!");
        return;
    }
    address += ":" + ui->editPort->text();

    if (ui->radioLocal->isChecked())
    {
        QStringList channels = ui->editChannels->text().trimmed().split(" ");
        QString nick = ui->editNick->text().trimmed();

        sConnections.saveConnection(new LocalConnectionDetails(label, address, channels, nick));
    }
    else if (ui->radioRemote->isChecked())
    {
        QString user = ui->editUser->text().trimmed();
        QString pass_hash = fIRC::getHashFor(ui->editPass->text().trimmed());

        sConnections.saveConnection(new RemoteConnectionDetails(label, address, user, pass_hash));
    }
    else
    {
        QDialog::accept();
        return;
    }

    emit done();

    QDialog::accept();
}
