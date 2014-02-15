#pragma once

#ifndef RECTANGLE2
#define RECTANGLE2

inline float Rectangle2::left() 
{
    return position.x;
}

inline float Rectangle2::right() 
{
    return position.x + dimensions.x;
}

inline float Rectangle2::top() 
{
     return position.y;
}

inline float Rectangle2::bottom() 
{
    return position.y + dimensions.y;
}


inline Vector2 Rectangle2::topLeft() 
{
    return position;   
}

inline Vector2 Rectangle2::topRight() 
{
     return Vector2(position.x  + dimensions.x, position.y);
}

inline Vector2 Rectangle2::botLeft() 
{
    return Vector2(position.x, position.y + dimensions.y);
}

inline Vector2 Rectangle2::botRight() 
{
    return position + dimensions;
}

#endif