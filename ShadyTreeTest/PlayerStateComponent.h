#pragma once
#include "Component.h"
#include "Timer.h"

class PlayerStateComponent : public Component
{
public: 
    PlayerStateComponent(void);
    PlayerStateComponent(int _id, bool _active);
    void Initialize();
    virtual void CloneFrom(Component* c);

    bool airborne;
    bool knocked;
    int jumpCount;
    int damage;

    float movementSpeed;
    float airborneAccel;
    float maxVelX;
    float maxVelY;
    float jumpVelocity;

    float jumpBonus;
    float moveBonus;

    float getMovementSpeed();
    float getJumpVelocity();

    Timer knockedTimer;
    Timer jumpTimer;
    Timer attackWait;
};
