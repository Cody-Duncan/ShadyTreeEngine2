#pragma once
#include "GameObject\Component.h"
#include "Resources\GraphicsResourceHandles.h"
#include "Math\MathLib.h"
#include "GameObject\ComponentTypeID.h"

class PrimitiveGraphicsComponent : public Component
{
public:
    ST_API PrimitiveGraphicsComponent(void);
    ST_API PrimitiveGraphicsComponent(int _id, bool _active);
    ST_API ~PrimitiveGraphicsComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    /// <summary>
    /// The layer for draw ordering.
    /// Layers 0-4 are prior to sprites.
    /// Layers 4-9 are after sprites.
    /// Layers 10-12 are reserved for debug
    /// </summary>
    int layer;
    Color color;
    Vector2 center;
    std::vector<Vector2> triangleListPoints;
private:
    void InitialValues();
};

RegisterType(PrimitiveGraphicsComponent)