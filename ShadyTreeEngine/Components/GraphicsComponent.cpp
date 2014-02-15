#include "StdAfx.h"
#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent(void)
{
}


GraphicsComponent::~GraphicsComponent(void)
{
}

void GraphicsComponent::CloneFrom(Component* _oldGC)
{
    GraphicsComponent* oldGC = dynamic_cast<GraphicsComponent*>(_oldGC);
    texture = oldGC->texture;
    textureArea = oldGC->textureArea ;
}
