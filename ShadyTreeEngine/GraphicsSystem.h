#pragma once

#include "ISystem.h"

class GraphicsDevice;
class SpriteBatch;
class PrimitiveBatch;

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

    ST_API void setClearColor(Color color);


private: 
    GraphicsDevice* device;
    SpriteBatch* spriteBatch;
    PrimitiveBatch* primitiveBatch;
};