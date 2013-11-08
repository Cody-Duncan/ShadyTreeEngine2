#pragma once

class Rectangle
{
    Rectangle();
    Rectangle(float posX, float posY, float width, float height);
    Rectangle(Vector2 position, Vector2 dimension);

    Vector2 position;
    Vector2 dimensions;
};