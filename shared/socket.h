#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>
#include "packet.h"


namespace Socket
{
    /*Opcode GetOpcode(QByteArray data);
    Opcode ExtractOpcode(QTcpSocket * sock);*/
    void write(QTcpSocket* socket, Opcode opcode = OPC_NULL, QByteArray data = QByteArray());
}

#endif // SOCKET_H
