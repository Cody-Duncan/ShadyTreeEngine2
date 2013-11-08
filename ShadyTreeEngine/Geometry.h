#pragma once

class Rectangle2
{
    Rectangle2();
    Rectangle2(float posX, float posY, float width, float height);
    Rectangle2(Vector2 position, Vector2 dimensions);

    Vector2 position;
    Vector2 dimensions;
};