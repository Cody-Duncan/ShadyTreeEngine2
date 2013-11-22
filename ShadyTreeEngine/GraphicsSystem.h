#pragma once

#include "ISystem.h"
#include "GraphicsDevice.h"
#include "SpriteBatch.h"

class GraphicsSystem : public ISystem
{
public:
    virtual void Init();
    virtual void Load();
    virtual void Update();
    virtual void Unload();

private: 
    GraphicsDevice* device;
    SpriteBatch* spriteBatch;
};