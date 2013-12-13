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

    movementSpeed = 20.0f;
    airborneAccel = 40.0f;
    jumpVelocity = 350.0f;
}