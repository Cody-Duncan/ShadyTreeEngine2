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
    damage = 0;

    movementSpeed = 15.0f;
    airborneAccel = 150.0;
    maxVelX = 900.0f;
    maxVelY = 4000.0f;
    jumpVelocity = 2000.0f;
}