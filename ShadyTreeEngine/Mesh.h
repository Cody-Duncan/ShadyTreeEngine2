#pragma once

#include "GraphicsResourceHandles.h"

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

    BufferHandle bufferHandle;

    ST_API MeshHandle handle();
};

