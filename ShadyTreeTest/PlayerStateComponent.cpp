#include "PlayerStateComponent.h"


PlayerStateComponent::PlayerStateComponent(void)
{
    Initialize();
}

PlayerStateComponent::PlayerStateComponent(int _id, bool _active)
{
    Initialize();
}

void PlayerStateComponent::Initialize()
{
    airborne = false;
    knocked = false;
    jumpCount = 0;
    damage = 0;

    movementSpeed = 9.0f;
    airborneAccel = 90.0;
    maxVelX = 500.0f;
    maxVelY = 1000.0f;
    jumpVelocity = 600.0f;
}

void PlayerStateComponent::CloneFrom(Component* _c)
{
    PlayerStateComponent* c = dynamic_cast<PlayerStateComponent*>(_c);

    airborne = c->airborne;
    knocked = c->knocked;
    jumpCount = c->jumpCount;
    damage = c->damage;

    movementSpeed = c->movementSpeed;
    airborneAccel = c->airborneAccel;
    maxVelX = c->maxVelX;
    maxVelY = c->maxVelY;
    jumpVelocity = c->jumpVelocity;
}