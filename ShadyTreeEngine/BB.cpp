#include "stdafx.h"
#include "BB.h"
#include "BB_Math.h"
#include "Contact.h"

BB::BB(void) 
{
}

BB::BB(BB_Type t): type(t) 
{
}

BB::~BB(void)
{
}

void BB::generateGeometry(std::vector<Vector2>& fillList)
{
    return;
}

Vector2 BB::centerOffset()
{
    return Vector2();
}

////////// BB_Circle ///////////////
BB_Circle::BB_Circle() : BB(Circle)
{

}

BB_Circle::~BB_Circle()
{

}

BB_Circle& BB_Circle::operator=(BB_Circle& rhs)
{
    if(this == &rhs)
        return *this;
    
    radius = rhs.radius;
    return *this;
}
BB* BB_Circle::clone()
{
    return new BB_Circle(*this);
}

void BB_Circle::generateGeometry(std::vector<Vector2>& fillList)
{
    float step = PI/8;
    for(float angle = 0.0f; angle < 360.0f; angle += step)
    {
        fillList.push_back(Vector2(0,0));
        fillList.push_back(Vector2(sin(angle+step), cos(angle+step))*radius);
        fillList.push_back(Vector2(sin(angle), cos(angle))*radius);
    }
}

Vector2 BB_Circle::centerOffset()
{
    return Vector2();
}

////////// BB_Rectangle ///////////////
BB_Rectangle::BB_Rectangle() : BB(Rect)
{

}

BB_Rectangle::~BB_Rectangle()
{

}

BB_Rectangle& BB_Rectangle::operator=(BB_Rectangle& rhs)
{
    if(this == &rhs)
        return *this;
    
    extents = rhs.extents;
    return *this;
}

BB* BB_Rectangle::clone()
{
    return new BB_Rectangle(*this);
}

void BB_Rectangle::generateGeometry(std::vector<Vector2>& fillList)
{
    fillList.push_back(Vector2(-extents.x, -extents.y));
    fillList.push_back(Vector2(extents.x, -extents.y));
    fillList.push_back(Vector2(extents.x, extents.y));

    fillList.push_back(Vector2(-extents.x, -extents.y));
    fillList.push_back(Vector2(extents.x, extents.y));
    fillList.push_back(Vector2(-extents.x, extents.y));
}

Vector2 BB_Rectangle::centerOffset()
{
    return Vector2();
}






bool collideBox_Circle   (BB*, Vector2&, BB*, Vector2&, Contact& c);
bool collideCircle_Circle(BB*, Vector2&, BB*, Vector2&, Contact& c);
bool collideBox_Box      (BB*, Vector2&, BB*, Vector2&, Contact& c);
bool collideCircle_Box   (BB*, Vector2&, BB*, Vector2&, Contact& c);

//Circle=0, Box=1, coded by BB_Type enum
bool (*collideTable[2][2]) (BB*, Vector2&, BB*, Vector2&, Contact& contactResult) = 
{
    {collideCircle_Circle, collideCircle_Box },
    {collideBox_Circle   , collideBox_Box    }
};


bool CollisionCheck(BB* a, Vector2 posA, BB* b, Vector2 posB, Contact& contactResult)
{
    return collideTable[a->type][b->type](a, posA, b, posB, contactResult);
}



bool collideCircle_Box(BB* c1, Vector2& cPos, BB* b2, Vector2& bPos, Contact& contactResult)
{
    BB_Circle* circ = static_cast<BB_Circle*>(c1);
    BB_Rectangle* rect = static_cast<BB_Rectangle*>(b2);

    if(StaticCircleToStaticRectangle(&cPos, circ->radius, &bPos, rect->extents.x, rect->extents.y, contactResult))
        return true;
    return false;
}

bool collideBox_Circle(BB* b1, Vector2& bPos, BB* c2, Vector2& cPos, Contact& contactResult)
{
    //collideBox_Circle always takes Circle as object A, Box as Object B
    //flip the contact normal for this function because
    //the ordering of the object collision is reversed.
    bool result = collideCircle_Box(c2, cPos, b1, bPos, contactResult);
    contactResult.ContactNormal = -contactResult.ContactNormal;
    return result;
}


bool collideCircle_Circle(BB* c1, Vector2& c1Pos, BB* c2, Vector2& c2Pos,  Contact& contactResult)
{
    BB_Circle* circ1 = static_cast<BB_Circle*>(c1);
    BB_Circle* circ2 = static_cast<BB_Circle*>(c2);

    if(StaticCircleToStaticCircle(&c1Pos, circ1->radius, &c2Pos, circ2->radius, contactResult) )
        return true;
    return false;
}

bool collideBox_Box(BB*b1, Vector2& b1Pos, BB* b2, Vector2& b2Pos,  Contact& contactResult)
{
    BB_Rectangle* rect1 = static_cast<BB_Rectangle*>(b1);
    BB_Rectangle* rect2 = static_cast<BB_Rectangle*>(b2);

    if(StaticRectToStaticRect(&b1Pos, rect1->extents.x, rect1->extents.y, &b2Pos, rect2->extents.x, rect2->extents.y, contactResult) )
        return true;
    return false;
}

