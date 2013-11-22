#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"

class GraphicsComponent : public Component
{
public:
    ST_API GraphicsComponent(void);
    ST_API GraphicsComponent(int _id, bool _active) : Component(_id, _active) {}
    ST_API ~GraphicsComponent(void);

    TextureHandle texture;
    Rectangle2 textureArea;
};
