#include "stdafx.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(void) : 
    IsStatic(true), 
    Mass(0.0f),
    InvMass(0.0f),
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
    Mass         = oldGC->Mass;
    InvMass      = oldGC->InvMass;
    velocity     = oldGC->velocity;
    acceleration = oldGC->acceleration;
    body         = oldGC->body->clone();
}
