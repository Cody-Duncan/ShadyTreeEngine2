#pragma once
#include "stdafx.h"
#include "DebugDrawComponent.h"

DebugDrawComponent::DebugDrawComponent(void)
{
    Initialize();
}

DebugDrawComponent::DebugDrawComponent(int _id, bool _active) : Component(_id, _active) 
{ 
    Initialize(); 
}

DebugDrawComponent::~DebugDrawComponent(void)
{
}

void DebugDrawComponent::CloneFrom(Component* _oldGC)
{
    DebugDrawComponent* oldGC = dynamic_cast<DebugDrawComponent*>(_oldGC);
    
    color = oldGC->color;
    geometry = oldGC->geometry;
    lines = oldGC->lines;
}

void DebugDrawComponent::Initialize()
{
    color = Color(1, 0, 0, 0.3f);
}