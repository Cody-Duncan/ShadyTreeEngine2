#pragma once
#include "Component.h"


class PlayerStateComponent : public Component
{
public: 
    PlayerStateComponent(void);
    PlayerStateComponent(int _id, bool _active);
    void Initialize();

    bool airborne;
    bool knocked;
    int damage;

    float movementSpeed;
    float airborneAccel;
    float maxVelX;
    float maxVelY;
    float jumpVelocity;

};
