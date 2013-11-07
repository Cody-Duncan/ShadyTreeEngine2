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
    int vertexCount;
    int indexCount;
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
    bool isDynamic;

    ID3D11Buffer* getVertexBuffer();
    ID3D11Buffer* getIndexBuffer();
};

struct MeshData
{
    int vertexOffset;
    int indexOffset;
    BufferHandle bufferHandle;
};

class BufferResourcer
{
public:
    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<ID3D11Buffer*> indexBuffers;

    std::unordered_map<std::string, int> meshIdByName;
    std::unordered_map<int, std::vector<int>> bufferToMeshes;
    std::unordered_map<int, MeshData> meshes;
    std::unordered_map<int, BufferData> buffers;

    int createVertexIndexBuffer(std::string name, Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount, ID3D11Device* device, BufferHandle* bufferHandle, MeshHandle* meshHandle );
    int createDynamicVertexIndexBuffer(std::string name, int vertexCount, int indexCount, ID3D11Device* device, BufferHandle* handle);

    BufferHandle getMeshByName(std::string name);
    void getMeshesByBuffer(BufferHandle h, std::vector<MeshHandle>& fillIDs);
    MeshData getMesh(MeshHandle h);
    BufferData getBuffer(BufferHandle h);

    void Dispose();

    static BufferResourcer& Instance()
    {
        static BufferResourcer b;
        return b;
    }

private:

    static int IDGen_mesh;
    static int IDGen_buffer;
    int generateMeshID();
    int generateBufferID();

    //Singleton
    BufferResourcer(void);
    ~BufferResourcer(void);
    BufferResourcer(BufferResourcer const&);              
    void operator=(BufferResourcer const&); 
};

