#include "stdafx.h"
#include "PhysicsDynamicComponent.h"


PhysicsDynamicComponent::PhysicsDynamicComponent(void)
{
}


PhysicsDynamicComponent::~PhysicsDynamicComponent(void)
{
}

void PhysicsDynamicComponent::CloneFrom(Component* _oldGC)
{
    PhysicsDynamicComponent* oldGC = dynamic_cast<PhysicsDynamicComponent*>(_oldGC);
}
