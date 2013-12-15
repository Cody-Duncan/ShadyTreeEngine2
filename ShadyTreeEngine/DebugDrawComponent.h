#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"
#include "ComponentTypeID.h"


class DebugDrawComponent : public Component
{
public:
    ST_API DebugDrawComponent(void);
    ST_API DebugDrawComponent(int _id, bool _active);
    ST_API ~DebugDrawComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    Color color;
    std::vector<Vector2> geometry;
    std::vector<Vector2> lines;
    Vector2 offset;
private:
    void Initialize();
};

RegisterType(DebugDrawComponent)

   