#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"
#include "ComponentTypeID.h"
#include "BB.h"

class PhysicsComponent : public Component
{



public:
    ST_API PhysicsComponent(void);
    ST_API PhysicsComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~PhysicsComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    bool IsStatic;

    Vector2 velocity;
    Vector2 acceleration;

    BB* body;
};

RegisterType(PhysicsComponent)