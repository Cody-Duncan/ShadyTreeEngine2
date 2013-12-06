#include "BB.h"
#include "Contact.h"

#ifndef MATH2D_H
#define MATH2D_H

struct LineSegment2D
{
    Vector2 mP0;		// Point on the line
    Vector2 mP1;		// Point on the line
    Vector2 mN;		    // Line's normal
    float mNdotP0;		// To avoid computing it every time it's needed

    /// <summary>
    /// Constructs a 2D line segment from two points.
    /// </summary>
    /// <param name="LS">The to-be-built line segment</param>
    /// <param name="Point0">Start point of the line segment</param>
    /// <param name="Point1">End point of the line Segment</param>
    /// <returns></returns>
    LineSegment2D(Vector2& Point0, Vector2& Point1)
    {
        mP0 = Point0;
        mP1 = Point1;
        Vector2 nresult = Point1 - Point0;       /* vector P0 to P1 */
        mN = Vector2(nresult.y, -nresult.x ); /* rotate -90 degrees */
        mN.Normalize();
        mNdotP0 = mN.Dot(Point0);
    }

};


//POINT - SHAPE
int StaticPointToStaticCircle(Vector2 *pP, Vector2 *pCenter, float Radius);
int StaticPointToStaticRect(Vector2 *pPos, Vector2 *pRect, float Width, float Height);

//CIRCLE - RECT
int StaticCircleToStaticCircle(Vector2 *pCenter0, float Radius0, Vector2 *pCenter1, float Radius1, Contact& c);
int StaticRectToStaticRect(Vector2 *pRect0, float Width0, float Height0, Vector2 *pRect1, float Width1, float Height1, Contact& c);
int StaticCircleToStaticRectangle(Vector2 *pCenter, float Radius, Vector2 *pRect, float Width, float Height, Contact& c);

//POINT - LINE
float StaticPointToStaticLineSegment(Vector2 *P, LineSegment2D *LS);
float AnimatedPointToStaticLineSegment(Vector2 *Ps, Vector2 *Pe, LineSegment2D *LS, Vector2 *Pi);
float AnimatedCircleToStaticLineSegment(Vector2 *Ps, Vector2 *Pe, float Radius, LineSegment2D *LS, Vector2 *Pi);
float ReflectAnimatedPointOnStaticLineSegment(Vector2 *Ps, Vector2 *Pe, LineSegment2D *LS, Vector2 *Pi, Vector2 *R);
float ReflectAnimatedCircleOnStaticLineSegment(Vector2 *Ps, Vector2 *Pe, float Radius, LineSegment2D *LS, Vector2 *Pi, Vector2 *R);

//POINT - CIRCLE
float AnimatedPointToStaticCircle(Vector2 *Ps, Vector2 *Pe, Vector2 *Center, float Radius, Vector2 *Pi);
float ReflectAnimatedPointOnStaticCircle(Vector2 *Ps, Vector2 *Pe, Vector2 *Center, float Radius, Vector2 *Pi, Vector2 *R);
float AnimatedCircleToStaticCircle(Vector2 *Center0s, Vector2 *Center0e, float Radius0, Vector2 *Center1, float Radius1, Vector2 *Pi);
float ReflectAnimatedCircleOnStaticCircle(Vector2 *Center0s, Vector2 *Center0e, float Radius0, Vector2 *Center1, float Radius1, Vector2 *Pi, Vector2 *R);

//////////////////////////
// Additional Functions //
//////////////////////////

/// <summary>
/// Compares two floats for closeness/virtual equality
/// </summary>
/// <param name="a">first float value</param>
/// <param name="b">second float value</param>
/// <param name="epsilon">The epsilon difference allowed for closeness</param>
/// <returns>True if the difference of the floats is less than epsilon.</returns>
inline bool compareFloat(float a, float b, float epsilon = 0.00001f)
{
    return abs(a-b) < epsilon;
}

#endif

#include "BB_Math.inl"