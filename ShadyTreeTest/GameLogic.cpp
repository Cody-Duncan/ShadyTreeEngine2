#include "GameLogic.h"
#include "Resources.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "PositionalComponent.h"

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
    Resources::Instance().LoadTextureFile("test", "resources/test.png");

    oneobject = GameObjectFactory::Instance().createGraphicalEntity();
    
    for(int i = 0; i < 20; ++i)
    {
        GameObjectFactory::Instance().createGraphicalEntity();
    }
    
}

void GameLogic::Update(float deltaTime)
{
    PositionalComponent* p = oneobject->getComponent<PositionalComponent>();
    p->position.x += 0.2f;
    p->position.y = sin(p->position.x/20) * 100.0f;
    p->rotation+=0.0f;
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
