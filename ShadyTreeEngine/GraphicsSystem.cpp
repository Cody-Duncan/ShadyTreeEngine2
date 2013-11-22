#include "GraphicsSystem.h"
#include "GraphicsFactory.h"

void GraphicsSystem::Init()
{
    //create the graphics device TODO, replace with graphics system
    device = generateGraphicsDevice(DeviceAPI::DirectX11);
    if( device->Init() )
    {
        device->Free();
        delete device;
        device = 0;
        DebugPrintF("Failed to create Graphics Device");
        assert(false && "Failed to create Graphics Device");
    }

    spriteBatch = generateSpriteBatch(device);
    spriteBatch->Init();
}

void GraphicsSystem::Load()
{
    
}

void GraphicsSystem::Update(float deltaTime)
{
    assert(spriteBatch && "no spritebatch initialized. Did you forget to call Init()?");
    spriteBatch->Begin();
        //spriteBatch->Draw(t, matrixArray[i], r);
    spriteBatch->End();
}

void GraphicsSystem::Unload()
{
    
}
