#pragma once
#include "Component.h"
#include "Timer.h"

enum PowerupType
{
    Speed,
    Jump,
    Whirl,
    Boom,
    Heal
};

class PowerupComponent : public Component
{
public: 
    PowerupComponent(void);
    PowerupComponent(int _id, bool _active);
    void Initialize();
    virtual void CloneFrom(Component* c);

    std::string powerType;
};