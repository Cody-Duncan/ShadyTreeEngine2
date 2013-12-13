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
    s.BuildArchetypes("Archetypes");
    level.Initialize("ArenaLevel", s);

    oneobject = GF.cloneArchetype("Player");
    
}

float scale = 0;
void GameLogic::Update(float deltaTime)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();

    if(!CF.hasComponentCache<PositionalComponent>())
        return;

    if(gINPUTSTATE->keyDown(VK_LEFT))
    {
        oneobject->getComponent<PositionalComponent>()->rotation += 0.01f;
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
