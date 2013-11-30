#include "stdafx.h"
#include "MeshResourcer.h"
#include "GraphicsResourceHandles.h"



int MeshResourcer::IDGen_mesh = 0;

MeshResourcer& MeshResourcer::Instance()
{
    static MeshResourcer m;
    return m;
}

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

Mesh* MeshResourcer::getMeshByName(std::string name)
{
    return meshes[meshIdByName[name]];
}

void MeshResourcer::Dispose()
{
    for( auto iter = meshes.begin(); iter != meshes.end(); iter++)
    {
        delete iter->second;
    }

    meshIdByName.clear();
    meshIdByName.swap(std::unordered_map<std::string, int>());
    meshes.clear();
    meshes.swap(std::unordered_map<int, Mesh*>());

    IDGen_mesh = 0;
}