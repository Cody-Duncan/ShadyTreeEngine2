#include "stdafx.h"
#include "MeshResourcer.h"
#include "GraphicsResourceHandles.h"



int MeshResourcer::IDGen_mesh = 0;

MeshResourcer::MeshResourcer(void)
{
}


MeshResourcer::~MeshResourcer(void)
{
}

MeshHandle MeshResourcer::generateMeshHandle(Mesh* mesh)
{
    MeshHandle m = {mesh->ID};
    return m;
}

Mesh* MeshResourcer::generateMesh(std::string name)
{
    int newMeshID = generateMeshID();

    Mesh* m = new Mesh();
    m->name = name;
    m->ID = newMeshID;

    meshes[newMeshID] = m;
    meshIdByName[name] = newMeshID;

    return m;
}

int MeshResourcer::generateMeshID()
{
    return IDGen_mesh++;
}


Mesh* MeshResourcer::getMesh(MeshHandle h)
{
    return meshes[h.meshID];
}

BufferHandle MeshResourcer::getMeshByName(std::string name)
{
    BufferHandle h = {meshIdByName[name]};
    return h;
}

void MeshResourcer::Dispose()
{
    for( auto iter = meshes.begin(); iter != meshes.end(); iter++)
    {
        delete iter->second;
    }

    meshIdByName.clear();
    meshes.clear();

    IDGen_mesh = 0;
}