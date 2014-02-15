#pragma once

#include "Messaging\Message.h"

class ISystem
{
public:
    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Unload() = 0;
    virtual void Free() = 0;
    virtual void RecieveMessage(Message* msg) = 0;
};