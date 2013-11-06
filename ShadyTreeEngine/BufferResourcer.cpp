#include "BufferResourcer.h"
#include "Debug_Graphics.h"

ID3D11Buffer* MeshData::getVertexBuffer() 
{
    return source->vertexBuffers[vertexBufferIndex];
}

ID3D11Buffer* MeshData::getIndexBuffer() 
{
    return source->indexBuffers[indexBufferIndex];
}


int BufferResourcer::IDGen_mesh = 0;

BufferResourcer::BufferResourcer(void)
{
    
}


BufferResourcer::~BufferResourcer(void)
{
    vertexBuffers.clear();
}

BufferHandle BufferResourcer::getMeshByName(std::string name)
{
    BufferHandle h = {meshIdByName[name]};
    return h;
}

BufferData BufferResourcer::getMesh(BufferHandle h)
{
    return meshes[h.bufferID];
}

int BufferResourcer::createVertexIndexBuffer(std::string name, Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount, ID3D11Device* device, BufferHandle* handle )
{
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( Vertex ) * vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;

    ID3D11Buffer* newVertexBuffer;
    HRESULT hr = device->CreateBuffer( &bd, &InitData, &newVertexBuffer );
    if( FAILED( hr ) )
        return hr;


    SetDebugObjectName(newVertexBuffer,"vertex buffer");
    vertexBuffers.push_back(newVertexBuffer);



    ID3D11Buffer* newIndexBuffer;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( unsigned int ) * indexCount;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = device->CreateBuffer( &bd, &InitData, &newIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    SetDebugObjectName(newIndexBuffer,"index buffer");
    indexBuffers.push_back(newIndexBuffer);

    BufferData h = {
        vertexBuffers.size()-1,                 //vertex buffer index
        indexBuffers.size()-1,                  //index buffer index
        this,                                   //source
        sizeof( Vertex ),                       //stride
        0,                                      //vertex offset
        0,                                      //index offset
        vertexCount,
        indexCount,                             //indexCount
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST   //primitiveTopology
    };

    int newID = generateID();
    meshes[newID] = h;
    meshIdByName[name] = newID;

    BufferHandle meshH = {newID};

    *handle = meshH;

    return 0;
}

int BufferResourcer::generateID()
{
    return IDGen_mesh++;
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

    meshIdByName.clear();
    meshes.clear();

    vertexBuffers.clear();
    indexBuffers.clear();

    IDGen_mesh = 0;
}