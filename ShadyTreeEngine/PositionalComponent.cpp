#include "stdafx.h"
#include "PositionalComponent.h"


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