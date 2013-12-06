#pragma once

struct Contact
{
    int ObjIDs[2];
    Vector2 Velocities[2];
    Vector2 ContactNormal;
    float Penetration;
};
