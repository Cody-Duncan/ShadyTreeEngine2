
/// <summary>
/// Determine collision between a point and static circle.
/// </summary>
/// <param name="pP">The point.</param>
/// <param name="pCenter">Position of the center of the circle.</param>
/// <param name="Radius">The radius of the circle.</param>
/// <returns>bool (as int)- true if colliding, false (0) if not colliding.</returns>
inline int StaticPointToStaticCircle(Vector2 *pP, Vector2 *pCenter, float Radius)
{
    /* (distance between P and Center)^2 <= Radius ^2 */
    return Vector2::DistanceSquared(*pP, *pCenter) <= (Radius*Radius);
}

/// <summary>
/// Determine collision between a point and static rectangle.
/// </summary>
/// <param name="pPos">The point.</param>
/// <param name="pRect">The center point of the rectangle.</param>
/// <param name="Width">The width of the rectangle.</param>
/// <param name="Height">The height of the rectangle.</param>
/// <returns>bool (as int)- true if colliding, false (0) if not colliding.</returns>
inline int StaticPointToStaticRect(Vector2 *pPos, Vector2 *pRect, float Width, float Height)
{
    float halfWidth = (Width/2.0f);
    float halfHeight = (Height/2.0f);

    if( pPos->x < pRect->x - halfWidth )
        return 0;
    if( pPos->x > pRect->x + halfWidth )
        return 0;
    if( pPos->y < pRect->y - halfHeight )
        return 0;
    if( pPos->y > pRect->y + halfHeight )
        return 0;
    return 1;
}

/// <summary>
/// Determine collision between a static circle and static circle.
/// </summary>
/// <param name="pCenter0">The center of circle0.</param>
/// <param name="Radius0">The radius of circle0</param>
/// <param name="pCenter1">The center of circle1.</param>
/// <param name="Radius1">The radius of circle1.</param>
/// <returns>bool (as int)- true if colliding, false (0) if not colliding.</returns>
inline int StaticCircleToStaticCircle(Vector2 *pCenter0, float Radius0, Vector2 *pCenter1, float Radius1, Contact& c)
{
    float radiusSum= (Radius0 + Radius1);
    Vector2 delta = *pCenter0 - *pCenter1;

    if( delta.LengthSquared() <= radiusSum*radiusSum)
    {
        c.ContactNormal = *pCenter0 - *pCenter1;//A to B
        c.Penetration = radiusSum - delta.Length();
        return true;
    }
    return false;
}

/// <summary>
/// Determine collision between a static rectangle and static rectangle.
/// </summary>
/// <param name="pRect0">The center point of rectangle0.</param>
/// <param name="Width0">The width of rectangle0.</param>
/// <param name="Height0">The height of rectangle0.</param>
/// <param name="pRect1">The center point of rectangle1.</param>
/// <param name="Width1">The width of rectangle1.</param>
/// <param name="Height1">The height of rectangle1.</param>
/// <returns>bool (as int)- true if colliding, false (0) if not colliding.</returns>
inline int StaticRectToStaticRect(Vector2 *pRect0, float Width0, float Height0, Vector2 *pRect1, float Width1, float Height1, Contact& c)
{
    //Separating Axis Theorem
    float xdiff = (Width0 + Width1) - fabs(pRect0->x - pRect1->x)*2;
    float ydiff = (Height0 + Height1) - fabs(pRect0->y - pRect1->y)*2;

    if( xdiff > 0 && ydiff > 0)
    {
        Vector2 delta = *pRect0 - *pRect1;
        if(xdiff < ydiff)
        {
            Vector2 normal = delta.x < 0 ? Vector2(-1,0) : Vector2(1,0);
            c.ContactNormal = normal;
            c.Penetration = xdiff/2;
            
        }
        else
        {
            Vector2 normal = delta.y < 0 ? Vector2(0,-1) : Vector2(0,1);
            c.ContactNormal = normal;
            c.Penetration = ydiff/2;
        }
        return true;
    }

    return false;

}


/// <summary>
/// Determine collision between a static circle and static rectangle.
/// </summary>
/// <param name="pCenter">The center of the circle.</param>
/// <param name="Radius">The radius of the circle.</param>
/// <param name="pRect">The corner of the rectangle.</param>
/// <param name="Width">The width of the rectangle.</param>
/// <param name="Height">The height of the rectangle.</param>
/// <returns>bool (as int), true for collision</returns>
inline int StaticCircleToStaticRectangle(Vector2 *pCenter, float Radius, Vector2 *pRect, float Width, float Height, Contact& c)
{
    
    Vector2& center = *pCenter;
    Vector2 point = center;
    Vector2 rectMin = Vector2(pRect->x - Width/2, pRect->y - Height/2);
    Vector2 rectMax = Vector2(pRect->x + Width/2, pRect->y + Height/2);

    //X
    if( center.x < rectMin.x )
    {
        point.x = rectMin.x;
    }
    else if( center.x > rectMax.x )
    {
        point.x = rectMax.x;
    }

    //Y
    if( center.y < rectMin.y )
    {
        point.y = rectMin.y;
    }
    else if( center.y > rectMax.y )
    {
        point.y = rectMax.y;
    }

    Vector2 delta = center - point;
    if(delta.LengthSquared() <= Radius*Radius)
    {
        c.ContactNormal = delta.Normal();
        c.Penetration = -(delta.Length() - Radius);

        return 1; 
    }
    return 0;
}

/// <summary>
/// Determines the distance separating a point from a line
/// </summary>
/// <param name="P">The point whose location should be determined</param>
/// <param name="LS">The line segment</param>
/// <returns>The distance of the point from the line. 
/// Negative = point is on inside half plane. 
/// Positive = point is on outside half plane. 
/// Zero = point is on the line
/// </returns>
inline float StaticPointToStaticLineSegment(Vector2 *P, LineSegment2D *LS)
{
    return LS->mN.Dot(*P) - LS->mNdotP0;
}


/// <summary>
/// Checks whether an animated point or circle is colliding with a line segment.
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="LS">The line segment</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <param name="r">radius of a circle (defaulted to 0, which represents a point).</param>
/// <returns>
/// Intersection time t.
/// -1.0f = No Intersection
/// </returns>
inline float AnimatedAreaToStaticLineSegment(Vector2 *Ps, Vector2 *Pe, LineSegment2D *LS, Vector2 *Pi, float r = 0)
{
    float N_Dot_Ps = LS->mN.Dot(*Ps);
    float N_Dot_Pe = LS->mN.Dot(*Pe);
    
    if(N_Dot_Ps < LS->mNdotP0) /*starts inside half plane*/
    {
        r = -r;
        if(N_Dot_Ps - LS->mNdotP0 < r &&  /* completly inside */
           N_Dot_Pe - LS->mNdotP0 < r) 
            return -1.0f;
    }
    else                       /*starts outside half plane*/
    {
        if(N_Dot_Ps - LS->mNdotP0 > r && /* completly outside */
           N_Dot_Pe - LS->mNdotP0 > r) 
            return -1.0f; 
    }

    /* calculate n dot v */
    Vector2 v = *Pe-*Ps;
    float n_Dot_v = LS->mN.Dot(v);

    if(n_Dot_v == 0.0f)                     /*parallel*/
        return -1.0f;
    
    /*calculate t and the collission point*/
    float t = (LS->mNdotP0 - N_Dot_Ps + r) / n_Dot_v; /*   t = (n.P0 - n.Ps)/n.v   */
    Vector2 collisionPoint = *Ps + v*t;             

    /* check that collision was on the line segment */
    Vector2& P0 = LS->mP0;
    Vector2& P1 = LS->mP1;
    if( (collisionPoint - P0).Dot(P1 - P0) < 0.0f)      /*collides with extension of wall off P0 */
        return -1.0f;
    if( (collisionPoint - P1).Dot(P0 - P1) < 0.0f)      /*collides with extension of wall off P1 */
        return -1.0f;

    *Pi = collisionPoint;
    return t;
}


/// <summary>
/// Checks whether an animated point is colliding with a line segment
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="LS">The line segment</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <returns>
/// Intersection time t.
/// -1.0f = No Intersection
/// </returns>
inline float AnimatedPointToStaticLineSegment(Vector2 *Ps, Vector2 *Pe, LineSegment2D *LS, Vector2 *Pi)
{
    return AnimatedAreaToStaticLineSegment(Ps, Pe, LS, Pi);
}


/// <summary>
/// Checks whether an animated circle is colliding with a line segment
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="Radius">The radius of the circle.</param>
/// <param name="LS">The line segment</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <returns>
/// Intersection time t.
/// -1.0f = No Intersection
/// </returns>
inline float AnimatedCircleToStaticLineSegment(Vector2 *Ps, Vector2 *Pe, float Radius, LineSegment2D *LS, Vector2 *Pi)
{
    return AnimatedAreaToStaticLineSegment(Ps, Pe, LS, Pi, Radius);
}


/// <summary>
/// Calculates the reflection vector of a point hitting a line segment.
/// </summary>
/// <param name="Pe">The point's original ending location.</param>
/// <param name="N">Normal vector to the line segment.</param>
/// <param name="Pi">Intersection point. Where the animated point collides with line segment.</param>
/// <param name="R">IN - Vector to fill with reflected vector based from Pi. </param>
inline void calculateLineSegmentReflection(Vector2 *Pe, Vector2* N, Vector2 *Pi, Vector2 *R)
{
    Vector2 i = *Pe - *Pi;
    *R = i - ( (*N) * 2*(i.Dot(*N)) ); /* R = i - 2(i.n)n */
}


/// <summary>
/// Calculates the reflection vector of an animated point reflecting off a line segment. 
/// Does nothing if there is no collision.
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="LS">The line segment</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <param name="R">IN - The will be used to store the reflection vector (In case there is a collision, and thus reflection).</param>
/// <returns>
/// Intersection time t.
/// -1.0f = No Intersection
/// </returns>
inline float ReflectAnimatedPointOnStaticLineSegment(Vector2 *Ps, Vector2 *Pe, LineSegment2D *LS, Vector2 *Pi, Vector2 *R)
{
    float t = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);
    if(t >= 0)
    {
        calculateLineSegmentReflection(Pe,&LS->mN, Pi, R);
    }

    return t;
}


/// <summary>
/// Reflects an animated circle off a static line segment.
/// Does nothing if there is no collision.
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="Radius">The radius of the circle.</param>
/// <param name="LS">The line segment</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <param name="R">IN - The will be used to store the reflection vector (In case there is a collision, and thus reflection).</param>
/// <returns>
/// Intersection time t.
/// -1.0f = No Intersection
/// </returns>
inline float ReflectAnimatedCircleOnStaticLineSegment(Vector2 *Ps, Vector2 *Pe, float Radius, LineSegment2D *LS, Vector2 *Pi, Vector2 *R)
{
    float t = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);
    if(t >= 0)
    {
        calculateLineSegmentReflection(Pe,&LS->mN, Pi, R);
    }

    return t;
}



/// <summary>
/// Checks whether an animated point is colliding with a static circle
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="Center">The static circle's center</param>
/// <param name="Radius">The static circle's radius</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <returns>
///     Intersection time t.
///     -1.0f = No Intersection
/// </returns>
inline float AnimatedPointToStaticCircle(Vector2 *Ps, Vector2 *Pe, Vector2 *Center, float Radius, Vector2 *Pi)
{
    static float noCollision = -1.0f;
    Vector2 V = *Pe-*Ps;
    Vector2 V_Norm = V.Normal();
    Vector2 Ps_C = *Center - *Ps;
    float RadiusSquared = Radius * Radius;

    //NonCollision 1: If m < 0 && Ps outside circle => circle behind ray origin
    float m = Ps_C.Dot(V_Norm);
    if(m < 0 && Vector2::DistanceSquared(*Ps, *Center) > RadiusSquared)
    {
        return noCollision;
    }

    //NonCollision 2: given   n^2 = magnitude(Bs_C)^2 - m^2
    //If   n^2 > radius^2   =>   ray will miss circle
    float nSquared = Ps_C.LengthSquared() - (m*m);
    if(nSquared > RadiusSquared)
    {
        return noCollision;
    }

    float s = sqrt(RadiusSquared - nSquared);
    float v_length = V.Length();
    float t0 = (m-s)/v_length;
    float t1 = (m+s)/v_length;


    //find t value closer to Ps
    float tmin = t0;
    float tmax = t1;
    if(t0 > t1)
    {
        tmin = t1;
        tmax = t0;
    }

    //make sure t value is within Ps to Pe, in other words: [0,1]
    float t;
    if( tmin < 0.0f || 1.0f < tmin)      //if tmin is outside Ps to Pe
    {
        if(tmax < 0.0f || 1.0f < tmax )  //if tmax is outside Ps to Pe
        {
            return noCollision;
        }
        else //tmax is good, use it
        {
            t = tmax;
        }
    }
    else //tmin is good, use it
    {
        t = tmin;
    }

    *Pi = *Ps + (V*t);
    return t;
    
}



/// <summary>
/// Calculates the reflection vector of a point hitting a static circle.
/// </summary>
/// <param name="Ps">Starting point of moving point to reflect.</param>
/// <param name="C">Center of the static circle.</param>
/// <param name="Pi">Intersection point with the circle.</param>
/// <param name="R">IN - Filled with reflected vector, based at Pi.</param>
inline void calculateStaticCircleReflection(Vector2 *Ps, Vector2* C, Vector2 *Pi, Vector2 *R)
{
    Vector2 n = (*Pi - *C).Normal();
    Vector2 m = *Ps - *Pi;
        
    //r = 2(m dot n)n - m
    *R = ( n*(m.Dot(n)*2) ) - m;
}



/// <summary>
/// Reflects ane animated point off static circle.
/// </summary>
/// <param name="Ps">The point's starting location</param>
/// <param name="Pe">The point's ending location</param>
/// <param name="Center">The static circle's center.</param>
/// <param name="Radius">The static circle's radius.</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <param name="R">IN - The will be used to store the reflection vector (In case there is a collision, and thus reflection).</param>
/// <returns>
///     Intersection time t.
///     -1.0f = No Intersection
/// </returns>
inline float ReflectAnimatedPointOnStaticCircle(Vector2 *Ps, Vector2 *Pe, Vector2 *Center, float Radius, Vector2 *Pi, Vector2 *R)
{
    float t = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);
    if( t >= 0.0f)
    {
        calculateStaticCircleReflection(Ps, Center, Pi, R);
    }

    return t;
}



/// <summary>
/// Checks whether an animated circle is colliding with a static circle
/// </summary>
/// <param name="Center0s">The starting position of the animated circle's center. </param>
/// <param name="Center0e">The ending position of the animated circle's center.</param>
/// <param name="Radius0">The animated circle's radius.</param>
/// <param name="Center1">The static circle's center.</param>
/// <param name="Radius1">The static circle's radius.</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <returns>
///     Intersection time t.
///     -1.0f = No Intersection
/// </returns>
inline float AnimatedCircleToStaticCircle(Vector2 *Center0s, Vector2 *Center0e, float Radius0, Vector2 *Center1, float Radius1, Vector2 *Pi)
{
    return AnimatedPointToStaticCircle(Center0s, Center0e, Center1, Radius0+Radius1, Pi);
}



/// <summary>
/// Reflects the animated circle on static circle.
/// </summary>
/// <param name="Center0s">The starting position of the animated circle's center. </param>
/// <param name="Center0e">The ending position of the animated circle's center.</param>
/// <param name="Radius0">The animated circle's radius.</param>
/// <param name="Center1">The static circle's center.</param>
/// <param name="Radius1">The static circle's radius.</param>
/// <param name="Pi">IN - This will be used to store the intersection point's coordinates (In case there's an intersection)</param>
/// <param name="R">IN - The will be used to store the reflection vector (In case there is a collision, and thus reflection).</param>
/// <returns>
///     Intersection time t.
///     -1.0f = No Intersection
/// </returns>
inline float ReflectAnimatedCircleOnStaticCircle(Vector2 *Center0s, Vector2 *Center0e, float Radius0, Vector2 *Center1, float Radius1, Vector2 *Pi, Vector2 *R)
{
    float t = AnimatedCircleToStaticCircle(Center0s, Center0e, Radius0, Center1, Radius1, Pi);
    if( t >= 0.0f)
    {
        calculateStaticCircleReflection(Center0s, Center1, Pi, R);
    }

    return t;
}
