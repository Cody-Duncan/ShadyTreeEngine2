
#include "MathLib.h"

#ifndef VERTEX_STRUCT
#define VERTEX_STRUCT

struct Vertex
{
    Vector4 Pos;
    Vector2 Tex;
};

struct VertexCol
{
    Vector4 Pos;
    Color Col;
}

#endif