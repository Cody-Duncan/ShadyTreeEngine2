#pragma once
#include "GameObject\Component.h"
#include "Math\MathLib.h"
#include "ST_API_Def.h"
#include "GameObject\ComponentTypeID.h"

class PositionalComponent : public Component
{
public:
    ST_API PositionalComponent(void);
    ST_API PositionalComponent(int _id, bool _active) : Component(_id, _active),
        position(Vector2(0,0)), rotation(0.0f), scale(1.0f), rotationCentered(false)
    {}
    ST_API ~PositionalComponent(void);
    ST_API virtual void CloneFrom(Component* _c);

    Vector2 position;
    float rotation;
    float scale;
    bool rotationCentered;

    Matrix Transform();
};

RegisterType(PositionalComponent)