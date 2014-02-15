#include "GameObjectFactory.h"
#include "Components\GraphicsComponent.h"
#include "Components\PositionalComponent.h"


GameObjectFactory::GameObjectFactory(void)
{
}


GameObjectFactory::~GameObjectFactory(void)
{
}

GameObject* GameObjectFactory::createGraphicalEntity(std::string textureResID, Vector2 centerPos)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    IResources& RES = Resources::Instance();

    GameObject* e = GOC.Create();
    
    GraphicsComponent* graphics = CF.createComponent<GraphicsComponent>();
    graphics->texture = RES.GetTexture(textureResID);

    graphics->textureArea.position = Vector2(0,0);
    graphics->textureArea.dimensions = RES.GetTextureWidthHeight(textureResID);

    PositionalComponent* posC = CF.createComponent<PositionalComponent>();
    posC->scale = 1.0f;
    posC->position = centerPos - (graphics->textureArea.dimensions*(0.5f));

    e->attachComponent(graphics);
    e->attachComponent(posC);

    return e;
}

void GameObjectFactory::addArchetype(std::string archetypeName, int id)
{
    if (!hasArchetype(archetypeName))
    {
        archetypes[archetypeName] = id;
    }
    else
    {
        DebugPrintf("Tried to overwrite an archetype: %s\n", archetypeName.c_str());
    }
}

GameObject* GameObjectFactory::cloneArchetype(std::string archetypeName)
{
    GameObject* newGO = nullptr;
    if (hasArchetype(archetypeName))
    {
        GameObjectCache& GOC = GameObjectCache::Instance();
        ComponentFactory& CF = ComponentFactory::Instance();
        IResources& RES = Resources::Instance();
        int archeid = archetypes[archetypeName];

        newGO = GOC.Create();
        newGO->CloneFrom(archeid);
    }
    return newGO;
}

bool GameObjectFactory::hasArchetype(std::string name)
{
    return archetypes.find(name) != archetypes.end();
}

void GameObjectFactory::addEnemyType(std::string archetypeName)
{
    enemyTypes.push_back(archetypeName);
}

void GameObjectFactory::addPowerupType(std::string archetypeName)
{
    powerupTypes.push_back(archetypeName);
}

void GameObjectFactory::clearArchetypes()
{
    DebugPrintf("GAMELOGIC: Destroying Archetypes\n");
    for(auto iter = archetypes.begin(); iter != archetypes.end(); ++iter)
    {
        GameObjectCache::Instance().DestroyNow(iter->second);
    }
    archetypes.clear();
}
void GameObjectFactory::clearSubtypes()
{
    enemyTypes.clear();
    powerupTypes.clear();
}