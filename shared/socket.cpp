#include "socket.h"


/*Opcode Socket::GetOpcode(QByteArray data)
{
    return Opcode(data.left(data.indexOf(' ')).toInt());
}

Opcode Socket::ExtractOpcode(QTcpSocket * sock)
{
    return GetOpcode(sock->readAll());
}*/

void Socket::write(QTcpSocket* socket, Opcode opcode, QByteArray data)
{
    QByteArray packetData = QByteArray::number(opcode) + " " + data;
    socket->write(packetData);
    socket->flush();
}
