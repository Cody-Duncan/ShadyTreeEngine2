#pragma once
#include "Component.h"
#include "Timer.h"


class AIComponent : public Component
{
public: 
    AIComponent(void);
    AIComponent(int _id, bool _active);
    void Initialize();
    virtual void CloneFrom(Component* c);

    std::string aiType;
};