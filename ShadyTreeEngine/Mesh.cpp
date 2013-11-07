#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

MeshHandle Mesh::handle()
{
    MeshHandle m = {ID};
    return m;
}