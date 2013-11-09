#pragma once

#include <d3d11_1.h>
#include "vertex.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "GraphicsResourceHandles.h"


class Mesh;
class BufferResourcer;

struct VertexBufferData
{
     int vertexBufferIndex;
     BufferResourcer* source;
     int stride;
     int vertexLength;
     D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
     int startVertex;

     bool isDynamic;
     ID3D11Buffer* getVertexBuffer();
};

struct IndexBufferData
{
    int indexBufferIndex;
    BufferResourcer* source;
    int indexLength;
    int startIndex;

    bool isDynamic;
    ID3D11Buffer* getIndexBuffer();
};



class BufferResourcer
{
public:
    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<ID3D11Buffer*> indexBuffers;

    std::unordered_map<int, VertexBufferData> VbufferData;
    std::unordered_map<int, IndexBufferData> IbufferData;

    int createStaticBuffers(Mesh& mesh, ID3D11Device* device, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf);

    int createDynamicVertexBuffer(int vertexLength, ID3D11Device* device, VertexBufferHandle* handle);
    int createDynamicIndexBuffer(int indexLength, ID3D11Device* device, IndexBufferHandle* handle);
    int addMeshToDynamicBuffer(Mesh& mesh, ID3D11DeviceContext* context, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf);


    VertexBufferData& getVBuffer(VertexBufferHandle h);
    IndexBufferData&  getIBuffer(IndexBufferHandle h);

    void Dispose();

    static BufferResourcer& Instance()
    {
        static BufferResourcer b;
        return b;
    }

private:

    static int IDGen_Vbuffer;
    int generateVBufferID();
    static int IDGen_Ibuffer;
    int generateIBufferID();

    //Singleton
    BufferResourcer(void);
    ~BufferResourcer(void);
    BufferResourcer(BufferResourcer const&);              
    void operator=(BufferResourcer const&); 
};

