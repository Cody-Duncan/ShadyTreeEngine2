/*
    Component Manager 4
        Key the ComponentCaches using type_index

        Advantages
            Types can be added at will.
            Uses standard correctly.

        Disadvantages
            Again uses hefty RTTI
            Uses C++x11 standards.
            UnorderedMap is slower than hash_map.
        
        Not really better than approach 1.

*/
#pragma once

#include <hash_map>
#include <vector>
#include <typeinfo>
#include "GameObject\Component.h"
#include "ComponentCache.h"
#include "GameObject\ComponentTypeID.h"

#define DEFAULT_CACHE_LENGTH 1024

class ComponentFactory
{
public:
    ST_API static ComponentFactory& Instance();

    template<class T>
    T* createComponent();

    template<class T>
    T* getComponent(int id);

    ST_API Component* getComponent(int type, int id);

    template<class T>
    void deleteComponent(int id);

    ST_API void deleteComponent(int id, int typeID);

    ST_API Component* cloneComponent(int typeID, int id);

    template<class T>
    void createComponentCache();

    template<class T>
    bool hasComponentCache();

    template<class T>
    ComponentCache<T>* getCache();

    ST_API void clearAllCaches();
    void FreeAllCaches();

    std::vector<AbstractComponentCache*> map;

private:

    ST_API ComponentFactory(void);
    ST_API ~ComponentFactory(void);
    ComponentFactory(ComponentFactory const& copy);				//not implemented
    ComponentFactory& operator=(ComponentFactory const& copy);	//not implemented
};


template<class T>
T* ComponentFactory::createComponent()
{

    if(!hasComponentCache<T>())
    {
        createComponentCache<T>();
    }

    int index = getID<T>();
    ComponentCache<T>* cc = static_cast<ComponentCache<T>*>(map[index]);
    T* newComp = cc->Generate();
    return newComp;
}

template<class T>
T* ComponentFactory::getComponent(int id)
{
     return getCache<T>()->Retrieve(id);
}

template<class T>
void ComponentFactory::deleteComponent(int id)
{
    deleteComponent(id, getID<T>());
}

template<class T>
void ComponentFactory::createComponentCache()
{
    unsigned int index = getID<T>();
    if(!hasComponentCache<T>()) //new indices are always by increment
    {
        ComponentCache<T>* cc = new ComponentCache<T>();
        cc->Reserve(DEFAULT_CACHE_LENGTH);
        if(index >= map.size())
            map.resize(index+1, nullptr);
        map[index] = cc;
    }
}

template<class T>
bool ComponentFactory::hasComponentCache()
{
    unsigned int index = getID<T>();
    if(index < map.size())
        return map[index] != nullptr;
    return false;
}

template<class T>
inline ComponentCache<T>* ComponentFactory::getCache()
{
    if(map.size() == 0 || !hasComponentCache<T>())
        return nullptr;
    int index = getID<T>();
    return static_cast<ComponentCache<T>*>(map[index]);
}