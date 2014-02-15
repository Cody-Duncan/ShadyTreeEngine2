#pragma once
#include "GameObject\Component.h"
#include "Utility\Timer.h"

class AIComponent : public Component
{
public: 
    AIComponent(void);
    AIComponent(int _id, bool _active);
    void Initialize();
    virtual void CloneFrom(Component* c);

    std::string aiType;
    int outerState;
    int innerState;

    Timer stateTimer;
    Timer attackTimer;

};