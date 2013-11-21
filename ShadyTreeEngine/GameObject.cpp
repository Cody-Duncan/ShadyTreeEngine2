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
    for(std::unordered_map<int, Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
    {
        ComponentFactory::Instance().deleteComponent(iter->second->id,iter->first);
    }
}