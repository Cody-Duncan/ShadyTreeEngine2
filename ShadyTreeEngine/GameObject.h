#pragma once

#include "Component.h"
#include "ComponentFactory.h"
#include "ComponentTypeID.h"

class GameObject
{
public:
    ST_API GameObject(void);
    ST_API ~GameObject(void);

    std::unordered_map<int, Component*> components;

    template<class T>
    void attachComponent(T* comp);

    template<class T>
    T* getComponent();

    template<class T>
    bool hasComponent();

    template<class T>
    void removeComponent();

    void clearComponents();

    int id;
    bool active;
};


template<class T>
void GameObject::attachComponent(T* comp)
{
    components.insert( std::make_pair<int,Component*>(getID<T>(), static_cast<Component*>(comp) ) );
}


template<class T>
T* GameObject::getComponent()
{
     return static_cast<T*>( components[getID<T>()] );
}


template<class T>
bool GameObject::hasComponent()
{
    return components.find(getID<T>()) != components.end();
}


template<class T>
void GameObject::removeComponent()
{
    int id = getID<T>();
    T* comp = static_cast<T*>( components[id] );
    components.erase( id );

    ComponentFactory::Instance().deleteComponent<T>(comp->id);
}

