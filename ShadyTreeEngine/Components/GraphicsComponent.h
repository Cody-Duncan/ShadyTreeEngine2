#pragma once
#include "GameObject\Component.h"
#include "Resources\GraphicsResourceHandles.h"
#include "Math\MathLib.h"
#include "GameObject\ComponentTypeID.h"

class GraphicsComponent : public Component
{
public:
    ST_API GraphicsComponent(void);
    ST_API GraphicsComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~GraphicsComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    TextureHandle texture;
    Rectangle2 textureArea;
};

RegisterType(GraphicsComponent)