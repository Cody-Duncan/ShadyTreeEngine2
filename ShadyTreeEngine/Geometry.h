#pragma once

class Rectangle2
{
public: 
    Rectangle2() : position(Vector2(0,0)), dimensions(Vector2(1,1)) {};

    Rectangle2(float posX, float posY, float width, float height) :
        position( Vector2(posX, posY) ), 
        dimensions( Vector2(width,height) ) {};

    Rectangle2(Vector2 position, Vector2 dimensions):
        position( position ), 
        dimensions( dimensions ) {};

    Vector2 position;
    Vector2 dimensions;

    float left();
    float right();
    float top();
    float bottom();

    Vector2 topLeft();
    Vector2 topRight();
    Vector2 botLeft();
    Vector2 botRight();
};

#include "Geometry.inl"