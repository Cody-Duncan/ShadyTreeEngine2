#pragma once
#include "GameObject\Component.h"
#include "Utility\Timer.h"

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
    Timer spawntimer;
    Timer powerTimer;
};