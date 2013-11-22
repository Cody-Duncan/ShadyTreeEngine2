#pragma once

#include "GraphicsResourceHandles.h"
#include "ST_API_Def.h"

class Mesh
{
public:
    Mesh(void);
    ~Mesh(void);

    int ID;

    int vertexOffset;
    int indexOffset;
    std::string name; 

    Vertex* vertices; 
    int vertexCount; 

    unsigned int* indices; 
    int indexCount;

    VertexBufferHandle hVBuffer;
    IndexBufferHandle hIBuffer;

    ST_API MeshHandle handle();
};

