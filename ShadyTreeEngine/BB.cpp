#include "stdafx.h"
#include "BB.h"
#include "BB_Math.h"

BB::BB(void) 
{
}

BB::BB(BB_Type t): type(t) 
{
}

BB::~BB(void)
{
}

////////// BB_Circle ///////////////
BB_Circle::BB_Circle() : BB(Circle)
{

}

BB_Circle::~BB_Circle()
{

}

////////// BB_Rectangle ///////////////
BB_Rectangle::BB_Rectangle() : BB(Rect)
{

}

BB_Rectangle::~BB_Rectangle()
{

}

bool collideBox_Circle(BB*, BB*);
bool collideCircle_Circle(BB*, BB*);
bool collideBox_Box(BB*, BB*);
bool collideCircle_Box(BB*, BB*);

//Circle=0, Box=1
bool (*collideTable[2][2]) (BB*, BB*) = 
{
    {collideCircle_Circle, collideCircle_Box },
    {collideBox_Circle   , collideBox_Box    }
};


bool collide(BB* a, BB* b)
{
    return collideTable[a->type][b->type](a, b);
}

bool collideCircle_Box(BB* c1, BB* b2)
{
    return collideBox_Circle(b2, c1);
}

bool collideBox_Circle(BB* b1, BB* c2)
{
    BB_Rectangle* rect = static_cast<BB_Rectangle*>(b1);
    BB_Circle* circ = static_cast<BB_Circle*>(c2);

    if( 0 < StaticCircleToStaticRectangle(&circ->center, circ->radius, &rect->low, rect->high.x, rect->high.y) )
        return true;
    return false;
}

bool collideCircle_Circle(BB* c1, BB* c2)
{
    BB_Circle* circ1 = static_cast<BB_Circle*>(c1);
    BB_Circle* circ2 = static_cast<BB_Circle*>(c2);

    if( 0 < StaticCircleToStaticCircle(&circ1->center, circ1->radius, &circ2->center, circ2->radius) )
        return true;
    return false;
}

bool collideBox_Box(BB*b1, BB* b2)
{
    BB_Rectangle* rect1 = static_cast<BB_Rectangle*>(b1);
    BB_Rectangle* rect2 = static_cast<BB_Rectangle*>(b2);
    if( 0 < StaticRectToStaticRect(&rect1->low, rect1->high.x, rect1->high.y, &rect2->low, rect2->high.x, rect2->high.y) )
        return true;
    return false;
}

