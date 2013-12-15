#pragma once
#include "Message.h"
#include "GameStateSystem.h"

class ChangeStateMessage : public Message
{
public:
    ChangeStateMessage(State _newState) : Message(ChangeState), newState(_newState){};
    State newState;
    virtual ~ChangeStateMessage(){};
};