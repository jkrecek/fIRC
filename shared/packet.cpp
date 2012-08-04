#include "packet.h"
#include <QDebug>

Packet::Packet(QTcpSocket *socket)
{
    QByteArray rawData = socket->readAll();
    qDebug() << "packet raw: " << rawData;
    int idx = rawData.indexOf(' ', 1);
    if (idx == -1)
    {
        opcode_m = Opcode(rawData.trimmed().toInt());
        data_m = QByteArray();
    }
    else
    {
        opcode_m = Opcode(rawData.left(idx).trimmed().toInt());
        data_m = rawData.mid(idx+1).trimmed();
    }



    //QList<QByteArray> dataParts = rawData.split(' ');
    //opcode_m = Opcode(dataParts[0].toInt());
    //dataParts.removeFirst();
    //data_m = dataParts.
}
