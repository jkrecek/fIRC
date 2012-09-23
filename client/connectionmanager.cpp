#include "connectionmanager.h"

#include <QCoreApplication>

#include <QDebug>
#include <firc.h>


ConnectionManager::ConnectionManager() :
    settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationDomain(), QCoreApplication::applicationName())
{
    loadConnections();
}

ConnectionManager::~ConnectionManager()
{
    foreach(ConnectionDetails* con, m_connections)
        delete con;
}

void ConnectionManager::loadConnections()
{
    settings.beginGroup(fIRC::Connection::ConnectionList);
    QStringList connectionLabels = settings.allKeys();
    settings.endGroup();

    foreach (QString connLabel, connectionLabels)
    {
        bool isValid = settings.value(fIRC::Connection::ConnectionList + "/" + connLabel).toBool();

        settings.beginGroup(getGroupPrefix(connLabel));

        ConnectionDetails * det = NULL;
        QString method = settings.value(fIRC::Connection::Method).toString();
        if (method == fIRC::Connection::ConnectionMethod::Local)
        {
            QString address = settings.value(fIRC::Connection::Address).toString();
            QStringList channels = settings.value(fIRC::Connection::Channels).toStringList();
            QString nick = settings.value(fIRC::Connection::Nick).toString();

            det = new LocalConnectionDetails(connLabel, address, channels, nick);
        }
        else if (method == fIRC::Connection::ConnectionMethod::Remote)
        {
            QString address = settings.value(fIRC::Connection::Address).toString();
            QString user = settings.value(fIRC::Connection::User).toString();
            QString pass_hash = settings.value(fIRC::Connection::Pass_Hash).toString();

            det = new RemoteConnectionDetails(connLabel, address, user, pass_hash);
        }

        if (isValid)
            m_connections.insert(connLabel, det);
        else
            m_connections_deleted.insert(connLabel, det);
    }
}

void ConnectionManager::saveConnection(ConnectionDetails *details)
{
    // save to memory
    m_connections.insert(details->m_label, details);

    // save to file
    settings.setValue(fIRC::Connection::ConnectionList + "/" + details->m_label, true);

    settings.beginGroup(getGroupPrefix(details->m_label));

    if (details->m_method == CON_LOCAL)
    {
        LocalConnectionDetails* loc_det = static_cast<LocalConnectionDetails*>(details);
        settings.setValue(fIRC::Connection::Method, fIRC::Connection::ConnectionMethod::Local);
        settings.setValue(fIRC::Connection::Address, loc_det->m_address);
        settings.setValue(fIRC::Connection::Channels, loc_det->m_channels);
        settings.setValue(fIRC::Connection::Nick, loc_det->m_nick);
    }
    else if (details->m_method == CON_REMOTE)
    {
        RemoteConnectionDetails* rem_det = static_cast<RemoteConnectionDetails*>(details);
        settings.setValue(fIRC::Connection::Method, fIRC::Connection::ConnectionMethod::Remote);
        settings.setValue(fIRC::Connection::Address, rem_det->m_address);
        settings.setValue(fIRC::Connection::User, rem_det->m_user);
        settings.setValue(fIRC::Connection::Pass_Hash, rem_det->m_pass_hash);
    }

    settings.endGroup();
}

QString ConnectionManager::getGroupPrefix(QString connectionLabel)
{
    return fIRC::Connection::ConnectionPrefix + connectionLabel;
}
