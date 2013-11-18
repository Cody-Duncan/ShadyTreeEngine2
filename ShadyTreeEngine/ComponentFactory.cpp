#include "ComponentFactory.h"


ComponentFactory::ComponentFactory(void)
{
    typeIDGen = 0;
}


ComponentFactory::~ComponentFactory(void)
{
}

int ComponentFactory::typeIDGen = 0;