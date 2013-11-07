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

    std::unordered_map<std::string, int> meshIdByName;
    std::unordered_map<int, std::vector<int>> bufferToMeshes;
    std::unordered_map<int, Mesh*> meshes;
    std::unordered_map<int, BufferData> buffers;

    int createVertexIndexBuffer(Mesh& mesh, ID3D11Device* device, BufferHandle* bufferHandle, MeshHandle* meshHandle );
    int createDynamicVertexIndexBuffer(int vertexLength, int indexLength, ID3D11Device* device, BufferHandle* handle);
    int addMeshToDynamicBuffer(Mesh& mesh, ID3D11DeviceContext* context, BufferHandle* bufferHandle, MeshHandle* meshHandle);

    BufferHandle getMeshByName(std::string name);
    void getMeshesByBuffer(BufferHandle h, std::vector<MeshHandle>& fillIDs);
    Mesh* getMesh(MeshHandle h);
    BufferData getBuffer(BufferHandle h);

    MeshHandle generateMeshHandle(Mesh* mesh);
    Mesh* generateMesh(std::string name);

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

