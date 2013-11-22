#include "GraphicsSystem.h"
#include "GraphicsFactory.h"
#include "ComponentFactory.h"
#include "GraphicsComponent.h"
#include "PositionalComponent.h"

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

    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.getCache<GraphicsComponent>()) //check for any graphicsComponents
        return;

    std::vector<GraphicsComponent>& graphC = CF.getCache<GraphicsComponent>()->storage;
    std::vector<PositionalComponent>& posC = CF.getCache<PositionalComponent>()->storage;


    spriteBatch->Begin();
        //spriteBatch->Draw(t, matrixArray[i], r);
    for(unsigned int i = 0; i < graphC.size(); i++)
    {
        if(graphC[i].active)
        {

        }
    }

    spriteBatch->End();
}

void GraphicsSystem::Unload()
{
    
}
