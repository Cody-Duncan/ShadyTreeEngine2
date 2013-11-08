#include "Geometry.h"

Rectangle2::Rectangle2(): position(Vector2(0,0)), dimensions(Vector2(1,1))
{

}

Rectangle2::Rectangle2(float posX, float posY, float width, float height) : 
    position( Vector2(posX, posY) ), 
    dimensions( Vector2(width,height) )
{

}

Rectangle2::Rectangle2(Vector2 position, Vector2 dimensions):
    position( position ), 
    dimensions( dimensions )
{

}