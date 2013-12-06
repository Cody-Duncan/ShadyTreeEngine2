#pragma once

enum BB_Type
{
    Circle,
    Rect
};

class BB
{
public:
    BB(void);
    virtual ~BB(void);

    BB_Type type;

protected:
     BB(BB_Type t);
};

class BB_Circle : public BB
{
public: 
    BB_Circle();
    ~BB_Circle();

    Vector2 center;
    float radius;
};

class BB_Rectangle : public BB
{
public:
    BB_Rectangle();
    ~BB_Rectangle();

    Vector2 low;
    Vector2 high;
};

bool collide(BB*, BB*);

extern bool (*collideTable[2][2]) (BB*, BB*);
