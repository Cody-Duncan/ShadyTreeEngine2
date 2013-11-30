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
