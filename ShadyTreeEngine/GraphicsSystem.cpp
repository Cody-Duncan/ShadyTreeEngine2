#include "GraphicsSystem.h"
#include "GraphicsFactory.h"
#include "ComponentFactory.h"
#include "GraphicsComponent.h"
#include "PositionalComponent.h"
#include "GameObject.h"
#include "GameObjectCache.h"

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

int count = 0;
double totalTime = 1.0f;
std::stringstream ss;
std::string resetString = "";
void GraphicsSystem::Update(float deltaTime)
{
    assert(spriteBatch && "no spritebatch initialized. Did you forget to call Init()?");

    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<GraphicsComponent>() || !CF.hasComponentCache<PositionalComponent>() ) //check for any graphicsComponents
        return;

    std::vector<GraphicsComponent>& graphC = CF.getCache<GraphicsComponent>()->storage;
    std::vector<PositionalComponent>* posC = &CF.getCache<PositionalComponent>()->storage;
    GameObjectCache& GOC = GameObjectCache::Instance();

    spriteBatch->Begin(true);
        //spriteBatch->Draw(t, matrixArray[i], r);
    for(unsigned int i = 0; i < graphC.size(); i++)
    {
        GraphicsComponent& g = graphC[i];
        if(g.active)
        {
            GameObject& go = *GOC.Get(g.parentID);
            PositionalComponent* posC = go.getComponent<PositionalComponent>();
            assert(posC && posC->active);

            spriteBatch->Draw(g.texture, posC->Transform(), g.textureArea);
        }
    }

    ++count;
    totalTime += deltaTime;
    char buf[100];
    sprintf_s(buf, "FPS: %8.2f", (count / totalTime * 1000));
    spriteBatch->TextDraw(Vector2(1,1), buf);
    ss.str(resetString);

    if(totalTime > 1000.0f)
    {
        count = 0;
        totalTime = 0;
    }

    spriteBatch->End();
}

void GraphicsSystem::Unload()
{
    
}

void GraphicsSystem::Free()
{
    spriteBatch->Dispose();
    device->Free();

    delete spriteBatch;
    delete device;
}

void GraphicsSystem::RecieveMessage(Message* msg)
{
    
}
