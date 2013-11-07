#include "BufferResourcer.h"
#include "Debug_Graphics.h"

ID3D11Buffer* BufferData::getVertexBuffer() 
{
    return source->vertexBuffers[vertexBufferIndex];
}

ID3D11Buffer* BufferData::getIndexBuffer() 
{
    return source->indexBuffers[indexBufferIndex];
}


int BufferResourcer::IDGen_mesh = 0;
int BufferResourcer::IDGen_buffer = 0;

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

void BufferResourcer::getMeshesByBuffer(BufferHandle h, std::vector<MeshHandle>& fillIDs)
{
    fillIDs.clear();
    std::vector<int>& idList = bufferToMeshes[h.bufferID];
    fillIDs.resize(idList.size());
    
    for(int i = 0; i < idList.size(); i++)
    {
        MeshHandle h = { idList[i] };
        fillIDs.push_back(h);
    }
}

MeshData BufferResourcer::getMesh(MeshHandle h)
{
    return meshes[h.meshID];
}

BufferData BufferResourcer::getBuffer(BufferHandle h)
{
    return buffers[h.bufferID];
}

int BufferResourcer::createVertexIndexBuffer(std::string name, Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount, ID3D11Device* device, BufferHandle* bufferHandle, MeshHandle* meshHandle  )
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

    BufferData  bufferD = {
        vertexBuffers.size()-1,                 //vertex buffer index
        indexBuffers.size()-1,                  //index buffer index
        this,                                   //source
        sizeof( Vertex ),                       //stride
        vertexCount,
        indexCount,                             //indexCount
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  //primitiveTopology
        true                                    //isDynamic
    };

    int newMeshID = generateMeshID();
    int newBufferID = generateBufferID();

    BufferHandle bufH = {newBufferID};
    MeshHandle meshH = {newMeshID};

    MeshData meshD = 
    {
        0,
        0,
        bufH
    };

    meshes[newMeshID] = meshD;              //store mesh data
    buffers[newBufferID] = bufferD;         //store buffer data
    meshIdByName[name] = newMeshID;         //reference mesh by name
    bufferToMeshes[newBufferID].push_back(newMeshID);  //reference mesh by buffer
    
    

    *bufferHandle = bufH;
    *meshHandle = meshH;

    return 0;
}

int BufferResourcer::createDynamicVertexIndexBuffer(std::string name, int vertexCount, int indexCount, ID3D11Device* device, BufferHandle* handle)
{
    //vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof( Vertex ) * vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ID3D11Buffer* newVertexBuffer;
    HRESULT hr = device->CreateBuffer( &bd, nullptr, &newVertexBuffer );
    if( FAILED( hr ) )
        return hr;


    SetDebugObjectName(newVertexBuffer,"dynamic vertex buffer");
    vertexBuffers.push_back(newVertexBuffer);


    //index buffer
    ID3D11Buffer* newIndexBuffer;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof( unsigned int ) * indexCount;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = device->CreateBuffer( &bd, nullptr, &newIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    SetDebugObjectName(newIndexBuffer,"dynamic index buffer");
    indexBuffers.push_back(newIndexBuffer);

    BufferData bufferD = {
        vertexBuffers.size()-1,                 //vertex buffer index
        indexBuffers.size()-1,                  //index buffer index
        this,                                   //source
        sizeof( Vertex ),                       //stride
        vertexCount,
        indexCount,                             //indexCount
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  //primitiveTopology
        true                                    //isDynamic
    };              

    int newBufferID = generateBufferID();
    buffers[newBufferID] = bufferD;

    BufferHandle bufferH = {newBufferID};

    *handle = bufferH;

    return 0;
}

int BufferResourcer::generateMeshID()
{
    return IDGen_mesh++;
}

int BufferResourcer::generateBufferID()
{
    return IDGen_buffer++;
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