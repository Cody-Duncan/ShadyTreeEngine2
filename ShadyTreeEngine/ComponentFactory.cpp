#include "ComponentFactory.h"


ComponentFactory& ComponentFactory::Instance()
{
    static ComponentFactory instance;
    return instance;
}

ComponentFactory::ComponentFactory(void)
{
    typeIDGen = 0;
}


ComponentFactory::~ComponentFactory(void)
{
}

int ComponentFactory::typeIDGen = 0;

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
    assert((unsigned int)typeID < map.size()); //make sure the cache exists
    
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
