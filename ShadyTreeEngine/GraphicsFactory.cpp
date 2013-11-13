#include "GraphicsFactory.h"

#include "DirectX_SpriteBatch.h"
#include "Resources.h"

GraphicsDevice* generateGraphicsDevice(DeviceAPI type)
{
    GraphicsDevice* device = nullptr;
    switch(type)
    {
    case DeviceAPI::DirectX11:
        device = new DirectX_GraphicsDevice();
    }

    Resources::Instance().setGraphicsDevice(device);
    return device;
}

SpriteBatch* generateSpriteBatch(GraphicsDevice* graphicsDevice)
{
    if( dynamic_cast<DirectX_GraphicsDevice*>(graphicsDevice) )
        return new DirectX_SpriteBatch(graphicsDevice);
    
    return nullptr;
}