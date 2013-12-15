#include "Message.h"

class AttachDebugDrawMessage : public Message
{
public:
    AttachDebugDrawMessage(int _objectID) : Message(DebugDrawGameObject), objectID(_objectID) {}
    int objectID;
};

class DebugDrawAllMessage : public Message
{
public:
    DebugDrawAllMessage() : Message(DebugDrawAllObjects) {}
};

class ToggleDebugDrawMessage : public Message
{
public:
    ToggleDebugDrawMessage() : Message(MessageType::ToggleDebugDraw) {}
};