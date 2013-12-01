#pragma once

#include "ST_API_Def.h"

class GameObject;

class ST_API Component
{
public:
    Component(void);
    Component(int _id, bool _active);
    ~Component(void);

    virtual void CloneFrom(Component* c) {};

    int id;
    bool active;
    int parentID;
};

