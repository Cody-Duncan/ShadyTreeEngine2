#include "stdafx.h"
#include "GameObject.h"


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
}

void  GameObject::CloneFrom(GameObject* g)
{
    ComponentFactory& CF = ComponentFactory::Instance();
    
    for(auto iter = g->components.begin(); iter != g->components.end(); ++iter)
    {
        components[iter->first] = CF.cloneComponent(iter->first, iter->second)->id;
    }
}