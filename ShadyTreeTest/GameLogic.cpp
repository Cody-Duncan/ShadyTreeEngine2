#include "GameLogic.h"
#include "Resources.h"
#include "GameObjectFactory.h"

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
}

void GameLogic::Update(float deltaTime)
{
    
}

void GameLogic::Unload()
{
    
}
