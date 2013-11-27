#include "GameLogic.h"
#include "Resources.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "PositionalComponent.h"
#include "InputState.h"
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
    IResources& res = Resources::Instance();

    res.parseResourceIDs("resources");
    res.LoadTextureRes("test");

    oneobject = GameObjectFactory::Instance().createGraphicalEntity();
    
    for(int i = 0; i < 20; ++i)
    {
        GameObjectFactory::Instance().createGraphicalEntity();
    }
    
}

void GameLogic::Update(float deltaTime)
{
    PositionalComponent* p = oneobject->getComponent<PositionalComponent>();
    
    if(gINPUTSTATE->keyHeld(VK_LEFT))
    {
        p->position.x += 0.2f;
        p->position.y = sin(p->position.x/20) * 100.0f;
        p->rotation+=0.01f;
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
