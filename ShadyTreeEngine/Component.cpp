#include "Component.h"


Component::Component(void) : id(-1), active(false)
{
}

Component::Component(int _id,  bool _active)
    : id(_id), active(_active)
{
}



Component::~Component(void)
{
}
