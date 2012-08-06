#include "packet.h"
#include <QDebug>

#include "ircconstants.h"

Packet::Packet(Opcode opcode, QByteArray data) : opcode_m(opcode), data_m(data)
{

}

Packet Packet::read(QByteArray data)
{
    qDebug() << "packet raw: " << data;
    int idx = data.indexOf(' ', 1);

    if (idx == -1)
        return Packet(Opcode(data.trimmed().toInt()), QByteArray());
    else
    {
        Opcode opc = Opcode(data.left(idx).trimmed().toInt());

        QString extracted = data.mid(idx+1).trimmed();
        extracted.replace(IRC::ALT_END, QString(IRC::END));

        return Packet(opc, extracted.toUtf8());
    }    
}

void Packet::write(QTcpSocket* socket, Opcode opcode, QByteArray data)
{
    if (data.endsWith(IRC::END))
        data.chop(IRC::END.size());

    if (!data.isEmpty())
        data = QString(data).replace(QString(IRC::END), IRC::ALT_END).toUtf8();

    QByteArray packetData = QByteArray::number(opcode) + " " + data;
    packetData.append(IRC::END);
    socket->write(packetData);
    socket->flush();
}

