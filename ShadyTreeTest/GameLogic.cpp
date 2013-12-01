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
    res.LoadTextureRes("test");

    oneobject = GF.createGraphicalEntity();
    
    for(int i = 0; i < 300; ++i)
    {
        GF.createGraphicalEntity();
    }

    DeSerializer s;
    s.BuildArchetypes("Player");
    
}

void GameLogic::Update(float deltaTime)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    for(unsigned int i = 0; i < GOC.entities.size(); ++i)
    {
        PositionalComponent* p = GOC.entities[i].getComponent<PositionalComponent>();
    
        if(gINPUTSTATE->keyHeld(VK_LEFT))
        {
            p->position.x += 0.002f;
            p->position.y = sin(p->position.x/20) * 100.0f;
            p->rotation+=0.0001f * i;
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
