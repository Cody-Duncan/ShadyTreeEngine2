#pragma once

#include <d3d11_1.h>
#include "vertex.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "GraphicsResourceHandles.h"


class BufferResourcer;

struct BufferData
{
    int vertexBufferIndex;
    int indexBufferIndex;
    BufferResourcer* source;
    int stride;
    int vertexOffset;
    int indexOffset;
    int vertexCount;
    int indexCount;
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

    ID3D11Buffer* getVertexBuffer();
    ID3D11Buffer* getIndexBuffer();
};

class BufferResourcer
{
public:
    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<ID3D11Buffer*> indexBuffers;

    std::unordered_map<std::string, int> meshIdByName;
    std::unordered_map<int, BufferData>meshes;

    int createVertexIndexBuffer(std::string name, Vertex* vertices, int vertexCount, unsigned int* indices, int numIndices, ID3D11Device* device, BufferHandle* handle );

    BufferHandle getMeshByName(std::string name);

    BufferData getMesh(BufferHandle h);

    void Dispose();

    static BufferResourcer& Instance()
    {
        static BufferResourcer b;
        return b;
    }

private:

    static int IDGen_mesh;
    int generateID();

    //Singleton
    BufferResourcer(void);
    ~BufferResourcer(void);
    BufferResourcer(BufferResourcer const&);              
    void operator=(BufferResourcer const&); 
};

