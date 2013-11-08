#include "GraphicsFactory.h"

GraphicsDevice* generateGraphicsDevice(DeviceAPI type)
{
    switch(type)
    {
    case DeviceAPI::DirectX11:
        return new DirectX_GraphicsDevice();
    default:
        return nullptr;
    }
}

SpriteBatch* generateSpriteBatch(GraphicsDevice* graphicsDevice)
{
    assert(false && "NOT IMPLEMENTED YET");
    return nullptr;
}