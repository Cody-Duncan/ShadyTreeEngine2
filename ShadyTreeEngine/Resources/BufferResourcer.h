#pragma once

#include <d3d11_1.h>
#include "Math\vertex.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Resources\GraphicsResourceHandles.h"


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

    std::vector<VertexBufferData> VbufferData;
    std::vector<IndexBufferData> IbufferData;

    int createStaticBuffers(Mesh& mesh, ID3D11Device* device, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf);

    int createDynamicVertexBuffer(int vertexLength, ID3D11Device* device, VertexBufferHandle* handle);
    int createDynamicIndexBuffer(int indexLength, ID3D11Device* device, IndexBufferHandle* handle);
    int addMeshToDynamicBuffer(Mesh& mesh, ID3D11DeviceContext* context, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf);


    VertexBufferData& getVBufferData(VertexBufferHandle h);
    IndexBufferData&  getIBufferData(IndexBufferHandle h);

    void Dispose();

    static BufferResourcer& Instance();

private:

    static int IDGen_Vbuffer;
    int generateVBufferID();
    static int IDGen_Ibuffer;
    int generateIBufferID();

    void checkVBufferSize(int vBufSize);
    void checkIBufferSize(int iBufSize);

    //Singleton
    BufferResourcer(void);
    ~BufferResourcer(void);
    BufferResourcer(BufferResourcer const&);              
    void operator=(BufferResourcer const&); 
};

