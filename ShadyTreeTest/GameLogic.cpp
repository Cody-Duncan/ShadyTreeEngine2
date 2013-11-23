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

    GameObjectFactory::Instance().createGraphicalEntity();

    int scale = 30;
    for(int i = 0; i < 10; i++)
    {
        GameObject* g = GameObjectFactory::Instance().createGraphicalEntity();
        PositionalComponent* p = g->getComponent<PositionalComponent>();
        p->position.x = 30*i;
        continue;
    }
}

void GameLogic::Update(float deltaTime)
{
    
}

void GameLogic::Unload()
{
    
}
