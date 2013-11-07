#pragma once

#include "Mesh.h"

class MeshResourcer
{
public:

    std::unordered_map<std::string, int> meshIdByName;
    
    std::unordered_map<int, Mesh*> meshes;

    MeshHandle generateMeshHandle(Mesh* mesh);
    Mesh* generateMesh(std::string name);

    Mesh* getMesh(MeshHandle h);
    BufferHandle getMeshByName(std::string name);

    void Dispose();

    static MeshResourcer& Instance()
    {
        static MeshResourcer m;
        return m;
    }

private:

    static int IDGen_mesh;
    int generateMeshID();

    //Singleton
    MeshResourcer(void);
    ~MeshResourcer(void);
    MeshResourcer(MeshResourcer const&);              
    void operator=(MeshResourcer const&); 
};

