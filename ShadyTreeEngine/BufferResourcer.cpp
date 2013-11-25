#include "BufferResourcer.h"
#include "Debug_Graphics.h"
#include "Mesh.h"

ID3D11Buffer* VertexBufferData::getVertexBuffer() 
{
    return source->vertexBuffers[vertexBufferIndex];
}

ID3D11Buffer* IndexBufferData::getIndexBuffer() 
{
    return source->indexBuffers[indexBufferIndex];
}





int BufferResourcer::IDGen_Vbuffer = 0;
int BufferResourcer::IDGen_Ibuffer = 0;

BufferResourcer& BufferResourcer::Instance()
{
    static BufferResourcer b;
    return b;
}

BufferResourcer::BufferResourcer(void)
{
    
}


BufferResourcer::~BufferResourcer(void)
{
    Dispose();
}


VertexBufferData& BufferResourcer::getVBuffer(VertexBufferHandle h)
{
   return VbufferData[h.VbufferID];
}

IndexBufferData&  BufferResourcer::getIBuffer(IndexBufferHandle h)
{
    return IbufferData[h.IbufferID];
}

int BufferResourcer::createStaticBuffers(Mesh& mesh, ID3D11Device* device, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf)
{
    //generate vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( Vertex ) * mesh.vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = mesh.vertices;

    ID3D11Buffer* newVertexBuffer;
    HRESULT hr = device->CreateBuffer( &bd, &InitData, &newVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    //store it
    SetDebugObjectName(newVertexBuffer,"vertex buffer");
    vertexBuffers.push_back(newVertexBuffer);


    //generate index buffer
    ID3D11Buffer* newIndexBuffer;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( unsigned int ) * mesh.indexCount;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = mesh.indices;
    hr = device->CreateBuffer( &bd, &InitData, &newIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    //store it
    SetDebugObjectName(newIndexBuffer,"index buffer");
    indexBuffers.push_back(newIndexBuffer);

    // create data references
    VertexBufferData dVBuf =
    {
        vertexBuffers.size()-1,                 //vertex buffer index
        this,                                   //source
        sizeof(Vertex),                         //stride
        mesh.vertexCount,                       //vertexLength
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  //topology
        0,                                      //startVertex
        false                                   //isDynamic
    };

    IndexBufferData dIBuf =
    {
        indexBuffers.size()-1,                //vertex buffer index
        this,                                 //source
        mesh.indexCount,                      //indexLength
        0,                                    //startIndex
        false                                 //isDynamic
    };
    
    //generate IDs
    int V_ID = generateVBufferID();
    int I_ID = generateIBufferID();

    assert(V_ID < 0 && "Vertex Buffer generated ID is less than 0. Check ID generation");

    //size vector to ensure index (key) is available
    checkVBufferSize(V_ID);
    checkIBufferSize(I_ID);

    //store data reference
    VbufferData[V_ID] = dVBuf;
    IbufferData[I_ID] = dIBuf;

    //set handles 
    VertexBufferHandle hV = { V_ID };
    IndexBufferHandle hI = { I_ID };

    *hVBuf = hV;
    *hIBuf = hI;

    //update mesh
    mesh.vertexOffset = 0;
    mesh.indexOffset = 0;
    mesh.hVBuffer = hV;
    mesh.hIBuffer = hI;

    return 0;
}


int BufferResourcer::createDynamicVertexBuffer(int vertexLength, ID3D11Device* device, VertexBufferHandle* handle)
{
    //vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DYNAMIC;                 //must be dynamic
    bd.ByteWidth = sizeof( Vertex ) * vertexLength;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     //dynamic requires write access

    ID3D11Buffer* newVertexBuffer;
    HRESULT hr = device->CreateBuffer( &bd, nullptr, &newVertexBuffer );
    CHECKHR(hr, "Failed to create dynamic index buffer");


    SetDebugObjectName(newVertexBuffer,"dynamic vertex buffer");
    vertexBuffers.push_back(newVertexBuffer);

     // create data references
    VertexBufferData dVBuf =
    {
        vertexBuffers.size()-1,                 //vertex buffer index
        this,                                   //source
        sizeof(Vertex),                         //stride
        vertexLength,                           //vertexLength
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  //topology
        0,                                      //startVertex
        true                                    //isDynamic
    };

    int V_ID = generateVBufferID();     //gen ID
    checkVBufferSize(V_ID);
    VbufferData[V_ID] = dVBuf;          //store data reference
    VertexBufferHandle hV = { V_ID };   //create and send back handle
    *handle = hV;

    return 0;
}

int BufferResourcer::createDynamicIndexBuffer(int indexLength, ID3D11Device* device, IndexBufferHandle* handle)
{
    //index buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DYNAMIC;                 //must be dynamic
    bd.ByteWidth = sizeof( unsigned int ) * indexLength;       
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     //dynamic requires write access

    ID3D11Buffer* newIndexBuffer;
    HRESULT hr = device->CreateBuffer( &bd, nullptr, &newIndexBuffer );
    CHECKHR(hr, "Failed to create dynamic index buffer");

    SetDebugObjectName(newIndexBuffer,"dynamic index buffer");
    indexBuffers.push_back(newIndexBuffer);

    IndexBufferData dIBuf =
    {
        indexBuffers.size()-1,                //vertex buffer index
        this,                                 //source
        indexLength,                          //indexLength
        0,                                    //startIndex
        false                                 //isDynamic
    };
    
    int I_ID = generateIBufferID();     //generate IDs
    checkIBufferSize(I_ID);
    IbufferData[I_ID] = dIBuf;          //store data reference
    IndexBufferHandle hI = { I_ID };    //set handle
    *handle = hI;
    
    return 0;
}

int BufferResourcer::addMeshToDynamicBuffer(Mesh& mesh, ID3D11DeviceContext* context, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf)
{
    VertexBufferData* VBufD = &VbufferData[ hVBuf->VbufferID ];
    ID3D11Buffer* vertexBuffer = VBufD->getVertexBuffer();

    IndexBufferData* IBufD = &IbufferData[ hIBuf->IbufferID ];
    ID3D11Buffer* indexBuffer = IBufD->getIndexBuffer();

    int startV = VBufD->startVertex;
    int startI = IBufD->startIndex;

    //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                           //map the buffer to lock resource
        Vertex* pV = (Vertex*) resource.pData;                                                      //convert data to Vertex* so we can set
        memcpy(&pV[VBufD->startVertex], mesh.vertices, sizeof( Vertex ) * mesh.vertexCount);        //memcopy the vertices in
    context->Unmap(vertexBuffer, 0);                                                                //unmap to unlock resource
    VBufD->startVertex += mesh.vertexCount;                                                         //increment startIndex(vertices)

    //update index buffer
    context->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                            //lock resource
        unsigned int* pI = (unsigned int *)resource.pData;                                          //convert to unsigned int*
        memcpy(&pI[IBufD->startIndex], mesh.indices, sizeof( unsigned int ) * mesh.indexCount);     //copy indices in
    context->Unmap(indexBuffer, 0);                                                                 //unlock resource
    IBufD->startIndex += mesh.indexCount;                                                           //increment startIndex(indices)

    return 0;
}

int BufferResourcer::generateVBufferID()
{
    return IDGen_Vbuffer++;
}

int BufferResourcer::generateIBufferID()
{
    return IDGen_Ibuffer++;
}

void BufferResourcer::checkVBufferSize(int vBufSize)
{
    if(VbufferData.size() <= (unsigned int) vBufSize)
        VbufferData.resize(vBufSize+5);
}

void BufferResourcer::checkIBufferSize(int iBufSize)
{
    if(IbufferData.size() <= (unsigned int) iBufSize)
        IbufferData.resize(iBufSize+5);
}

void BufferResourcer::Dispose()
{
    for( auto iter = vertexBuffers.begin(); iter != vertexBuffers.end(); iter++)
    {
        (*iter)->Release();
    }

    for( auto iter = indexBuffers.begin(); iter != indexBuffers.end(); iter++)
    {
        (*iter)->Release();
    }

    
    VbufferData.clear();
    IbufferData.clear();

    vertexBuffers.clear();
    indexBuffers.clear();

    IDGen_Vbuffer = 0;
    IDGen_Ibuffer = 0;
}