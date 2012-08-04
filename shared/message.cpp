#include "ircconstants.h"
#include "message.h"

Message::Message() :
    command_m(QString()),
    content_m(QString()),
    code_m(0),
    isServerMessage_m(false),
    isChannelMessage_m(false)
{
}

bool Message::isPing() const
{
    return (command_m == IRC::Command::Ping);
}

bool Message::isPrivate() const
{
    return (senderChannel_m == senderNick_m); // also possible: !(senderChannel_m.startsWith("#"))
}

