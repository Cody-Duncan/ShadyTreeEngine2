#pragma once

struct Contact
{
    //COLLISSION DATA
    //ID's of the objects that collided
    int ObjIDs[2];     

    //velocities of the objects as they collided
    Vector2 Velocities[2];

    //the normal to the contact
    Vector2 ContactNormal;

    //the penetration distance
    float Penetration;

    float Restitution;


    //RESOLUTION DATA
    //How far each object moved to stop penetrating
    Vector2 Movement[2];   

    float ContactImpulse;
};
