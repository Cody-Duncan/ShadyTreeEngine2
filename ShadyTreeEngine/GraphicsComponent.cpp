#include "stdafx.h"
#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent(void)
{
}


GraphicsComponent::~GraphicsComponent(void)
{
}

void GraphicsComponent::CloneInto(GraphicsComponent* newGC)
{
    newGC->texture = texture;
    newGC->textureArea = textureArea;
}
