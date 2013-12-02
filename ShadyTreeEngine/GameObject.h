#pragma once

#include "Component.h"
#include "ComponentFactory.h"
#include "ComponentTypeID.h"

class GameObject
{
public:
    ST_API GameObject(void);
    ST_API ~GameObject(void);

    //map of componentTypeID to componentID
    std::unordered_map<int, int> components;

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

    ST_API void CloneFrom(int ObjectID);
};


template<class T>
void GameObject::attachComponent(T* comp)
{
    int id = (int)comp->id;
    components.insert( std::make_pair(getID<T>(), id ) );
    comp->parentID = this->id;
}


template<class T>
T* GameObject::getComponent()
{
    int id = components[getID<T>()];
    return ComponentFactory::Instance().getComponent<T>(id);
}


template<class T>
bool GameObject::hasComponent()
{
    return components.find(getID<T>()) != components.end();
}


template<class T>
void GameObject::removeComponent()
{
    int type = getID<T>();
    int id = components[type];
    components.erase( type );

    ComponentFactory::Instance().deleteComponent<T>(id);
}

