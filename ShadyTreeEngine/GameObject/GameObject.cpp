#include "StdAfx.h"
#include "GameObject.h"
#include "GameObject\GameObjectCache.h"
#include "Messaging\Message.h"


GameObject::GameObject(void)
{
}


GameObject::~GameObject(void)
{
}

void GameObject::clearComponents()
{
    for(std::unordered_map<int, int>::iterator iter = components.begin(); iter != components.end(); ++iter)
    {
        ComponentFactory::Instance().deleteComponent(iter->second,iter->first);
    }
    components.clear();
}

void  GameObject::CloneFrom(int ObjectID)
{
    ComponentFactory& CF = ComponentFactory::Instance();
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* g = GOC.Get(ObjectID);
    
    for(auto iter = g->components.begin(); iter != g->components.end(); ++iter)
    {
        Component* c = CF.cloneComponent(iter->first, iter->second);
        components[iter->first] = c->id;
        c->parentID = id;
    }
}
