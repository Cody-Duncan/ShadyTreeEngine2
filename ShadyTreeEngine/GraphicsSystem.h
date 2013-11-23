#pragma once

#include "ISystem.h"
#include "GraphicsDevice.h"
#include "SpriteBatch.h"

class GraphicsSystem : public ISystem
{
public:
    ST_API GraphicsSystem() : device(0), spriteBatch(0) {};
    ~GraphicsSystem(void) {};

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();
    virtual void Free();
    virtual void RecieveMessage(Message* msg);

private: 
    GraphicsDevice* device;
    SpriteBatch* spriteBatch;
};