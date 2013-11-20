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
#include "Component.h"
#include "AbstractComponentCache.h"
#include "ComponentCache.h"

#define DEFAULT_CACHE_LENGTH 512

class ComponentFactory
{
public:
    static ComponentFactory& instance()
    {
        static ComponentFactory instance;
        return instance;
    }

    template<class T>
    T* createComponent();

    template<class T>
    T* getComponent(int id);

    template<class T>
    void deleteComponent(int id);


    template<class T>
    void createComponentCache();

    template<class T>
    bool hasComponentCache();

    template<class T>
    ComponentCache<T>* getCache();

    std::vector<AbstractComponentCache*> map;

private:

    static int typeIDGen;

    template<class T>
    static int getID();
    

    ComponentFactory(void);
    ~ComponentFactory(void);
    ComponentFactory(ComponentFactory const& copy);				//not implemented
    ComponentFactory& operator=(ComponentFactory const& copy);	//not implemented
};



template<class T>
int ComponentFactory::getID()
{
    static int idKey = typeIDGen++;
    return idKey;
}

template<class T>
T* ComponentFactory::createComponent()
{

    if(!hasComponentCache<T>())
    {
        createComponentCache<T>();
    }

    int index = getID<T>();
    ComponentCache<T>* ComponentCache = static_cast<ComponentCache<T>*>(map[index]);
    T* newComp = ComponentCache->Generate();
    return newComp;
}

template<class T>
T* ComponentFactory::getComponent(int id)
{
     getCache<T>()->Retrieve(id);
}

template<class T>
void ComponentFactory::deleteComponent(int id)
{
    getCache<T>()->Delete(id);
}

template<class T>
void ComponentFactory::createComponentCache()
{
    int index = getID<T>();
    if(index == map.size()) //new indices are always by increment
    {
        ComponentCache<T>* ComponentCache = new ComponentCache<T>();
        ComponentCache->Reserve(DEFAULT_CACHE_LENGTH);
        map.push_back(ComponentCache);
    }
}

template<class T>
bool ComponentFactory::hasComponentCache()
{
    int index = getID<T>();
    return index < map.size() ;
}

template<class T>
inline ComponentCache<T>* ComponentFactory::getCache()
{
    int index = getID<T>();
    return static_cast<ComponentCache<T>*>(map[index]);
}