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
    virtual BB* clone() = 0;

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
    virtual BB* clone();

    float radius;
};

class BB_Rectangle : public BB
{
public:
    BB_Rectangle();
    ~BB_Rectangle();
    BB_Rectangle& operator=(BB_Rectangle& rhs);
    virtual BB* clone();

    Vector2 extents;
};


struct Contact;

bool CollisionCheck(BB* a, Vector2 posA, BB* b, Vector2 posB, Contact& contactResult);

extern bool (*collideTable[2][2]) (BB*, Vector2&, BB*, Vector2&, Contact& contactResult);
