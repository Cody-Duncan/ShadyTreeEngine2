#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"
#include "ComponentTypeID.h"

class PhysicsStaticComponent : public Component
{
public:
    ST_API PhysicsStaticComponent(void);
    ST_API PhysicsStaticComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~PhysicsStaticComponent(void);

    ST_API virtual void CloneFrom(Component* c);

};

RegisterType(PhysicsStaticComponent)