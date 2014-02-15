#include "GraphicsFactory.h"

#include "Graphics\DirectX\DirectX_SpriteBatch.h"
#include "Graphics\DirectX\DirectX_PrimitiveBatch.h"
#include "Resources\Resources.h"

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

PrimitiveBatch* generatePrimitiveBatch(GraphicsDevice* graphicsDevice)
{
    if( dynamic_cast<DirectX_GraphicsDevice*>(graphicsDevice) )
        return new DirectX_PrimitiveBatch(graphicsDevice);
    
    return nullptr;
}