#pragma once
#include "Messaging\Message.h"

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

class DrawTextMessage : public Message
{
    public:
        DrawTextMessage(std::string _text, Vector2 _pos) : Message(MessageType::DebugDrawText), text(_text), pos(_pos) {}
        std::string text;
        Vector2 pos;
};