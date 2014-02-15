#include "StdAfx.h"
#include "Components\PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(void) 
{
    InitialValues();
}

PhysicsComponent::PhysicsComponent(int _id, bool _active) : Component(_id, _active) 
{
    InitialValues();
}

PhysicsComponent::~PhysicsComponent(void)
{
    delete body;
}

void PhysicsComponent::CloneFrom(Component* _oldGC)
{
    PhysicsComponent* oldGC = dynamic_cast<PhysicsComponent*>(_oldGC);

    IsStatic     = oldGC->IsStatic;
    Mass         = oldGC->Mass;
    InvMass      = oldGC->InvMass;
    velocity     = oldGC->velocity;
    acceleration = oldGC->acceleration;
    body         = oldGC->body ? oldGC->body->clone() : nullptr;
}

void PhysicsComponent::InitialValues()
{
    IsStatic = true;
    IsGhost = false;
    Mass = 0.0f;
    InvMass = 0.0f;
    Restitution = 0.0f;
    velocity = Vector2(0,0);
    acceleration = Vector2(0,0);
    offset = Vector2(0,0);
    body = nullptr;
}


void PhysicsComponent::CollideEvent(ContactMessage* m)
{
    for(auto iter = collisionDelegates.begin(); iter != collisionDelegates.end(); ++iter)
    {
        if(iter->IsValid())
            (*iter)(m);
    }
}

void PhysicsComponent::clearCollideHandlers()
{
    collisionDelegates.clear();
}