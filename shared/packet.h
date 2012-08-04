#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
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

class Packet
{
    public:
        Packet(QTcpSocket* socket);

        Opcode opcode()     const { return opcode_m; }
        QByteArray data()   const { return data_m; }
    private:
        Opcode opcode_m;
        QByteArray data_m;
};

#endif // PACKET_H
