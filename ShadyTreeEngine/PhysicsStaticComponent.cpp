#include "stdafx.h"
#include "PhysicsStaticComponent.h"


PhysicsStaticComponent::PhysicsStaticComponent(void)
{
}


PhysicsStaticComponent::~PhysicsStaticComponent(void)
{
}

void PhysicsStaticComponent::CloneFrom(Component* _oldGC)
{
    PhysicsStaticComponent* oldGC = dynamic_cast<PhysicsStaticComponent*>(_oldGC);
}
