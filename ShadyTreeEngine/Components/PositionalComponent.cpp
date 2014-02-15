#include "StdAfx.h"
#include "Components\PositionalComponent.h"


PositionalComponent::PositionalComponent(void) : position(Vector2(0,0)), rotation(0.0f), scale(1.0f)
{
}


PositionalComponent::~PositionalComponent(void)
{
}

Matrix PositionalComponent::Transform()
{
    return Matrix::CreateRotationZ(rotation)
         * Matrix::CreateScale(scale)
         * Matrix::CreateTranslation(position.x, position.y, 0);
}

 void PositionalComponent::CloneFrom(Component* _oldPC)
{
    PositionalComponent* oldPC = dynamic_cast<PositionalComponent*>(_oldPC);
    position = oldPC->position;
    rotation = oldPC->rotation;
    scale = oldPC->scale;
    rotationCentered = oldPC->rotationCentered;
}