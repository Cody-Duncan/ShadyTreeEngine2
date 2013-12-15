#pragma once
#include "Component.h"
#include "Timer.h"

enum AttackDir
{
    Attack_Left,
    Attack_Up,
    Attack_Right,
    Attack_Down
};

enum AttackType
{
    Regular,
    Airborne,
    Laser,
    Explosion
};

class AttackComponent : public Component
{
public: 
    AttackComponent(void);
    AttackComponent(int _id, bool _active);
    void Initialize();
    virtual void CloneFrom(Component* c);

    int ownerID;
    int damage;
    AttackDir direction;
    AttackType type;
    Timer attackTimer;
};