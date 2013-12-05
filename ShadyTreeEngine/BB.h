#pragma once

enum BB_Type
{
    Circle,
    Square
};

class BB
{
public:
    BB(void) {};
    BB(BB_Type t) : type(t) {}
    ~BB(void) {};

    BB_Type type;
};

