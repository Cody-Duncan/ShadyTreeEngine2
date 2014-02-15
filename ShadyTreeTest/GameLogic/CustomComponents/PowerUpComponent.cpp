#include "PowerupComponent.h"

PowerupComponent::PowerupComponent(void)
{
    Initialize();
}

PowerupComponent::PowerupComponent(int _id, bool _active)
{
    Initialize();
}

void PowerupComponent::Initialize()
{
    powerType = "";
}

void PowerupComponent::CloneFrom(Component* _c)
{
    PowerupComponent* c = dynamic_cast<PowerupComponent*>(_c);
    powerType = c->powerType;
}