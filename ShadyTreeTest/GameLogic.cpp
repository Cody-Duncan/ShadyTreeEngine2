#include "GameLogic.h"
#include "InputState.h"

#include "GraphicsComponent.h"
#include "PositionalComponent.h"

#include "DeSerializer.h"

GameLogic::GameLogic(void)
{
}


GameLogic::~GameLogic(void)
{
}

void GameLogic::Init()
{
    
}

void GameLogic::Load()
{
    DebugPrintf("GAME: Loading Game Logic\n");

    GameObjectCache::Instance().Reserve(400);
    GameObjectFactory& GF = GameObjectFactory::Instance();
    IResources& res = Resources::Instance();

    res.parseResourceIDs("resources");
    DeSerializer s;
    s.BuildArchetypes("Player");

    oneobject = GF.cloneArchetype("Player");
    GF.cloneArchetype("Player")->getComponent<PositionalComponent>()->position.x += 150;

    GF.cloneArchetype("Player")->getComponent<PositionalComponent>()->position.x += 300;
    GF.cloneArchetype("Player")->getComponent<PositionalComponent>()->position.x += 450;
    GF.cloneArchetype("Player")->getComponent<PositionalComponent>()->position.x += 600;
    GF.cloneArchetype("Player")->getComponent<PositionalComponent>()->position.x += 750;
    
}

float scale = 0;
void GameLogic::Update(float deltaTime)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();

    if(!CF.hasComponentCache<PositionalComponent>())
        return;

    std::vector<PositionalComponent>& posCache = CF.getCache<PositionalComponent>()->storage;
    for(unsigned int i = 0; i < posCache.size(); ++i)
    {
        PositionalComponent& p = posCache[i];
    
        if(gINPUTSTATE->keyHeld(VK_LEFT))
        {
            //p.position.x += 7.0f * deltaTime;
            //p.position.y += sin(p.position.x) * 100.0f * deltaTime;
            p.rotation += scale * i * deltaTime;
            scale+=0.001f;
        }
        else
        {
            p.rotation += scale * i * deltaTime;
            scale *= 0.99991f;
        }
    }
}

void GameLogic::Unload()
{
    
}

void GameLogic::Free()
{

}

void GameLogic::RecieveMessage(Message* msg)
{

}
