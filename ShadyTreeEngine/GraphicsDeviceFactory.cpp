#include "GraphicsDeviceFactory.h"

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