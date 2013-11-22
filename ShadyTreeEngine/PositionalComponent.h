#pragma once
#include "component.h"
#include "MathLib.h"
#include "ST_API_Def.h"

class PositionalComponent : public Component
{
public:
    ST_API PositionalComponent(void);
    ST_API PositionalComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~PositionalComponent(void);

    Vector2 position;
    float rotation;
    float scale;
};
