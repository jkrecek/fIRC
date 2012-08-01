#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>

enum Opcode
{
    OPC_NULL                = 0x00,
    OPC_LOGIN               = 0x01,
    OPC_WRONGLOGIN          = 0x02,
    OPC_LOGGED              = 0x03,
    OPC_ALREADYLOGGED       = 0x04,
    OPC_FORCELOGIN          = 0x05,
    OPC_INVALIDCMD          = 0x06,
    OPC_TOOMANYWRONGLOGINS  = 0x07,
    NUM_OPC                 = 0x08
};

namespace Socket
{
    Opcode GetOpcode(QByteArray data);
    Opcode ExtractOpcode(QTcpSocket * sock);
    void write(QTcpSocket* socket, Opcode opcode = OPC_NULL, QByteArray data = QByteArray());
}

#endif // SOCKET_H
