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



void ComponentFactory::deleteComponent(int id, int typeID)
{
    map[typeID]->Delete(id);
}

