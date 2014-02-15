#include "StdAfx.h"
#include "Resources\Mesh.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
    delete[] vertices;
    delete[] indices;
}

MeshHandle Mesh::handle()
{
    MeshHandle m = {ID};
    return m;
}