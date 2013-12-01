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
    GameObjectCache::Instance().Reserve(400);
    GameObjectFactory& GF = GameObjectFactory::Instance();
    IResources& res = Resources::Instance();

    res.parseResourceIDs("resources");
    DeSerializer s;
    s.BuildArchetypes("Player");

    oneobject = GF.cloneArchetype("Player");
    
    /*for(int i = 0; i < 300; ++i)
    {
        GF.createGraphicalEntity();
    }*/

    
    
}

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
            p.position.x += 0.002f;
            p.position.y = sin(p.position.x/20) * 100.0f;
            p.rotation+=0.0001f * i;
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
