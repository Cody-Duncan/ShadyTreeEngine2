#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"
#include "ComponentTypeID.h"

class PrimitiveGraphicsComponent : public Component
{
public:
    ST_API PrimitiveGraphicsComponent(void);
    ST_API PrimitiveGraphicsComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~PrimitiveGraphicsComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    int layer;
    Color color;
    Vector2 center;
    std::vector<Vector2> triangleListPoints;

};

RegisterType(PrimitiveGraphicsComponent)