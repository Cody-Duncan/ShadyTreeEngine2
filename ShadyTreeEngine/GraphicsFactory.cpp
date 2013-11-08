#include "GraphicsFactory.h"

#include "DirectX_SpriteBatch.h"

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
    if( dynamic_cast<DirectX_GraphicsDevice*>(graphicsDevice) )
        return new DirectX_SpriteBatch(graphicsDevice);
    
    return nullptr;
}