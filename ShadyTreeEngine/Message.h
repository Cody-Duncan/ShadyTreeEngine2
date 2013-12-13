#pragma once

enum MessageType
{
    None,
    Quit,
    Collision
};

class Message
{
public:
    Message(MessageType id) : type(id){};
    MessageType type;
    virtual ~Message(){};
};

