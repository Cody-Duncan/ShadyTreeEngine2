#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"
#include "ComponentTypeID.h"

class PhysicsDynamicComponent : public Component
{
public:
    ST_API PhysicsDynamicComponent(void);
    ST_API PhysicsDynamicComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~PhysicsDynamicComponent(void);

    ST_API virtual void CloneFrom(Component* c);

};

RegisterType(PhysicsDynamicComponent)