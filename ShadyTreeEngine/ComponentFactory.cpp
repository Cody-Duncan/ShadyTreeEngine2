#include "ComponentFactory.h"


ComponentFactory& ComponentFactory::Instance()
{
    static ComponentFactory instance;
    return instance;
}

ComponentFactory::ComponentFactory(void)
{
}


ComponentFactory::~ComponentFactory(void)
{
}

Component* ComponentFactory::getComponent(int type, int id)
{
    return map[type]->GetComponent(id);
}

void ComponentFactory::deleteComponent(int id, int typeID)
{
    map[typeID]->Delete(id);
}

Component* ComponentFactory::cloneComponent(int typeID, int id)
{
    DebugAssert( (unsigned int)typeID < map.size(),
        "ERROR: Trying to clone a component type that doesn't exist. \n"
        "If the typeID < map.size(), that means there is not a cache that yet exists to contain that type of component.");
    
    AbstractComponentCache* cache = map[typeID];
    Component* newComp = cache->Generate();
    Component* oldComp = cache->GetComponent(id);
    newComp->CloneFrom(oldComp);
    return newComp;
}

void ComponentFactory::clearAllCaches()
{
    for( std::vector<AbstractComponentCache*>::iterator iter = map.begin(); iter != map.end(); ++iter)
    {
        (**(iter)).Clear();
    }
}

void ComponentFactory::FreeAllCaches()
{
    for( std::vector<AbstractComponentCache*>::iterator iter = map.begin(); iter != map.end(); ++iter)
    {
        (**(iter)).Free();
        delete (*iter);
    }
    map.clear();
}
