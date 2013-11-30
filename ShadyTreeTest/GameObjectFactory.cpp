#include "GraphicsComponent.h"
#include "PositionalComponent.h"


GameObjectFactory::GameObjectFactory(void)
{
}


GameObjectFactory::~GameObjectFactory(void)
{
}

GameObject* GameObjectFactory::createGraphicalEntity()
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    IResources& RES = Resources::Instance();

    GameObject* e = GOC.Create();
    
    GraphicsComponent* graphics = CF.createComponent<GraphicsComponent>();
    graphics->texture = RES.GetTexture("test");

    graphics->textureArea.position = Vector2(0,0);
    graphics->textureArea.dimensions = RES.GetTextureWidthHeight("test");

    PositionalComponent* posC = CF.createComponent<PositionalComponent>();
    posC->position = Vector2(10,10);
    posC->rotation = 0.2f;
    posC->scale = 1.0f;

    e->attachComponent(graphics);
    e->attachComponent(posC);

    return e;
}

void GameObjectFactory::addArchetype(std::string archetypeName, int id)
{
    if(archetypes.find(archetypeName) != archetypes.end())
    {
        archetypes[archetypeName] = id;
    }
    else
    {
        DebugPrintf("Tried to overwrite an archetype");
    }
}