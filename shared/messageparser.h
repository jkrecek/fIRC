#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <QString>
#include "message.h"

class MessageParser
{
    public:
        static QString messageToRawText(const Message& message, const QByteArray& defaultCodec = QByteArray("UTF-8"));
        static Message receivedRawTextToMessage(const QByteArray& rawText, const QByteArray& defaultCodec = QByteArray("UTF-8"));

    private:
        static QString decodeRawText(const QByteArray& rawText, const QByteArray& defaultCodec = QByteArray("UTF-8"));
        MessageParser();
};

#endif // MESSAGEPARSER_H
