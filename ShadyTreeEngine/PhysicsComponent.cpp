#include "stdafx.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(void) : 
    IsStatic(true), 
    velocity(Vector2(0,0)), 
    acceleration(Vector2(0,0)),
    body(nullptr)
{
}


PhysicsComponent::~PhysicsComponent(void)
{
}

void PhysicsComponent::CloneFrom(Component* _oldGC)
{
    PhysicsComponent* oldGC = dynamic_cast<PhysicsComponent*>(_oldGC);

    IsStatic     = oldGC->IsStatic;
    velocity     = oldGC->velocity;
    acceleration = oldGC->acceleration;
    body         = oldGC->body;
}
