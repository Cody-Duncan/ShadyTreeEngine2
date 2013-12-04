#pragma once
#include "stdafx.h"
#include "PrimitiveGraphicsComponent.h"

PrimitiveGraphicsComponent::PrimitiveGraphicsComponent(void)
{
}


PrimitiveGraphicsComponent::~PrimitiveGraphicsComponent(void)
{
}

void PrimitiveGraphicsComponent::CloneFrom(Component* _oldGC)
{
    PrimitiveGraphicsComponent* oldGC = dynamic_cast<PrimitiveGraphicsComponent*>(_oldGC);
    
    layer = oldGC->layer;
    color = oldGC->color;
    center = oldGC->center;
    triangleListPoints = oldGC->triangleListPoints;
}
