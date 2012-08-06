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
    OPC_REQUEST_CONNECTION  = 0x08,
    OPC_MESSAGE_RECIEVED    = 0x09,
    OPC_IRC_SEND            = 0x0A,
    NUM_OPC
};

class Packet
{
    public:
        Packet(Opcode opcode, QByteArray data);

        static void write(QTcpSocket* socket, Opcode opcode = OPC_NULL, QByteArray data = QByteArray());
        static Packet read(QByteArray data);

        Opcode opcode()     const { return opcode_m; }
        QByteArray data()   const { return data_m; }
    private:
        Opcode opcode_m;
        QByteArray data_m;
};

#endif // PACKET_H
