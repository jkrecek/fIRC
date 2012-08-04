#include "packet.h"
#include <QDebug>

/*Packet::Packet(QTcpSocket *socket)
{
    QByteArray rawData = socket->readAll();
    //Packet(rawData);
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
}*/

Packet::Packet(QByteArray data)
{
    //QByteArray rawData = socket->readLine();
    qDebug() << "packet raw: " << data;
    int idx = data.indexOf(' ', 1);
    if (idx == -1)
    {
        opcode_m = Opcode(data.trimmed().toInt());
        data_m = QByteArray();
    }
    else
    {
        opcode_m = Opcode(data.left(idx).trimmed().toInt());
        data_m = data.mid(idx+1).trimmed();
    }



    //QList<QByteArray> dataParts = rawData.split(' ');
    //opcode_m = Opcode(dataParts[0].toInt());
    //dataParts.removeFirst();
    //data_m = dataParts.
}

