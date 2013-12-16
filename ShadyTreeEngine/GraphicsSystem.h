#pragma once

#include "ISystem.h"
#include "DebugDrawMessage.h"

class GraphicsDevice;
class SpriteBatch;
class PrimitiveBatch;

class GraphicsSystem : public ISystem
{
public:
    ST_API GraphicsSystem() : device(0), spriteBatch(0), debugDraw(false) {};
    ~GraphicsSystem(void) {};

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();
    virtual void Free();
    virtual void RecieveMessage(Message* msg);

    ST_API void setClearColor(Color color);

    virtual bool IsDebugDrawOn();
    virtual bool ToggleDebugDraw();

private: 
    GraphicsDevice* device;
    SpriteBatch* spriteBatch;
    PrimitiveBatch* primitiveBatch;

    bool debugDraw;

    std::vector<DrawTextMessage> textDrawQueue;
};