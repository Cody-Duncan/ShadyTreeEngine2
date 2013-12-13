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

    movementSpeed = 12.0f;
    airborneAccel = 120.0;
    maxVelX = 720.0f;
    maxVelY = 4000.0f;
    jumpVelocity = 1000.0f;
}