#pragma once

#include <d3d11_1.h>
#include "vertex.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "GraphicsResourceHandles.h"


class Mesh;
class BufferResourcer;

struct BufferData
{
    int vertexBufferIndex;
    int indexBufferIndex;
    BufferResourcer* source;

    int stride;
    int vertexLength;
    int indexLength;
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
    bool isDynamic;
    int startIndex;
    int startVertex;

    ID3D11Buffer* getVertexBuffer();
    ID3D11Buffer* getIndexBuffer();
};



class BufferResourcer
{
public:
    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<ID3D11Buffer*> indexBuffers;

    std::unordered_map<int, BufferData> buffers;
    std::unordered_map<int, std::vector<int>> bufferToMeshes;

    int createVertexIndexBuffer(Mesh& mesh, ID3D11Device* device, BufferHandle* bufferHandle);

    int createDynamicVertexIndexBuffer(int vertexLength, int indexLength, ID3D11Device* device, BufferHandle* handle);
    int addMeshToDynamicBuffer(Mesh& mesh, ID3D11DeviceContext* context, BufferHandle* bufferHandle);

    void getMeshesByBuffer(BufferHandle h, std::vector<MeshHandle>& fillIDs);

    BufferData getBuffer(BufferHandle h);

    void Dispose();

    static BufferResourcer& Instance()
    {
        static BufferResourcer b;
        return b;
    }

private:

    static int IDGen_buffer;
    int generateBufferID();

    //Singleton
    BufferResourcer(void);
    ~BufferResourcer(void);
    BufferResourcer(BufferResourcer const&);              
    void operator=(BufferResourcer const&); 
};

