#pragma once
#include "StdAfx.h"
#include "PrimitiveGraphicsComponent.h"

PrimitiveGraphicsComponent::PrimitiveGraphicsComponent(void)
{
    InitialValues();
}

PrimitiveGraphicsComponent::PrimitiveGraphicsComponent(int _id, bool _active) : Component(_id, _active)
{
    InitialValues();
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

void PrimitiveGraphicsComponent::InitialValues()
{
    layer = 0;
    color = Color(1,1,1,1);
    center = Vector2(0,0);
}
