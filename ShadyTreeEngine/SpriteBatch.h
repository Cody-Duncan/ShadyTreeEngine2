#pragma once
#pragma once

#include "DeviceAPI.h"
#include "GraphicsResourceHandles.h"

class ST_API SpriteBatch
{
public:
    SpriteBatch();
    SpriteBatch(DeviceAPI api) : API(api) {}
    virtual ~SpriteBatch(void) = 0;

    DeviceAPI API;

    virtual void Init() = 0;

    virtual void Begin() = 0;
    virtual void Draw(TextureHandle texH, Matrix transform, Rectangle2 rect) = 0;
    virtual void End() = 0;

    virtual void Dispose() = 0;

    virtual void DrawBatch(TextureHandle t) = 0;
};

