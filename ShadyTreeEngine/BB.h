#pragma once

enum BB_Type
{
    Circle = 0,
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
    BB_Circle& operator=(BB_Circle& rhs);

    float radius;
};

class BB_Rectangle : public BB
{
public:
    BB_Rectangle();
    ~BB_Rectangle();
    BB_Rectangle& operator=(BB_Rectangle& rhs);

    Vector2 extents;
};


struct Contact;

bool CollisionCheck(BB* a, Vector2 posA, BB* b, Vector2 posB, Contact& contactResult);

extern bool (*collideTable[2][2]) (BB*, Vector2&, BB*, Vector2&, Contact& contactResult);
