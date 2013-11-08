#include "BufferResourcer.h"
#include "Debug_Graphics.h"
#include "Mesh.h"

ID3D11Buffer* BufferData::getVertexBuffer() 
{
    return source->vertexBuffers[vertexBufferIndex];
}

ID3D11Buffer* BufferData::getIndexBuffer() 
{
    return source->indexBuffers[indexBufferIndex];
}


int BufferResourcer::IDGen_buffer = 0;

BufferResourcer::BufferResourcer(void)
{
    
}


BufferResourcer::~BufferResourcer(void)
{
    vertexBuffers.clear();
}


void BufferResourcer::getMeshesByBuffer(BufferHandle h, std::vector<MeshHandle>& fillIDs)
{
    fillIDs.clear();
    std::vector<int>& idList = bufferToMeshes[h.bufferID];
    fillIDs.resize(idList.size());
    
    for(unsigned int i = 0; i < idList.size(); i++)
    {
        MeshHandle h = { idList[i] };
        fillIDs.push_back(h);
    }
}


BufferData& BufferResourcer::getBuffer(BufferHandle h)
{
    return buffers[h.bufferID];
}

int BufferResourcer::createVertexIndexBuffer(Mesh& mesh, ID3D11Device* device, BufferHandle* bufferHandle)
{
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


    SetDebugObjectName(newVertexBuffer,"vertex buffer");
    vertexBuffers.push_back(newVertexBuffer);



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

    SetDebugObjectName(newIndexBuffer,"index buffer");
    indexBuffers.push_back(newIndexBuffer);

    BufferData  bufferD = {
        vertexBuffers.size()-1,                 //vertex buffer index
        indexBuffers.size()-1,                  //index buffer index
        this,                                   //source
        sizeof( Vertex ),                       //stride
        mesh.vertexCount,
        mesh.indexCount,                             //indexCount
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  //primitiveTopology
        true,                                   //isDynamic
        0,
        0
    };

    

    int newBufferID = generateBufferID();

    buffers[newBufferID] = bufferD;         //store buffer data
    bufferToMeshes[newBufferID].push_back(mesh.ID);  //reference mesh by buffer

    BufferHandle bufH = {newBufferID};
    *bufferHandle = bufH;

    mesh.vertexOffset = 0;
    mesh.indexOffset = 0;
    mesh.bufferHandle = bufH;

    return 0;
}

int BufferResourcer::createDynamicVertexIndexBuffer(int vertexLength, int indexLength, ID3D11Device* device, BufferHandle* handle)
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
    if( FAILED( hr ) )
        return hr;


    SetDebugObjectName(newVertexBuffer,"dynamic vertex buffer");
    vertexBuffers.push_back(newVertexBuffer);


    //index buffer
    ID3D11Buffer* newIndexBuffer;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DYNAMIC;                 //must be dynamic
    bd.ByteWidth = sizeof( unsigned int ) * indexLength;       
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     //dynamic requires write access

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
        vertexLength,                               
        indexLength,                            
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  //primitiveTopology
        true,                                   //isDynamic
        0,
        0
    };              

    //store the new buffer data
    int newBufferID = generateBufferID();
    buffers[newBufferID] = bufferD;

    //send back a handle
    BufferHandle bufferH = {newBufferID};
    *handle = bufferH;

    return 0;
}

int BufferResourcer::addMeshToDynamicBuffer(Mesh& mesh, ID3D11DeviceContext* context, BufferHandle* bufferHandle)
{
    
    BufferData* bufferData = &buffers[bufferHandle->bufferID];
    ID3D11Buffer* vertexBuffer = bufferData->getVertexBuffer();
    ID3D11Buffer* indexBuffer = bufferData->getIndexBuffer();

    int startV = bufferData->startVertex;
    int startI = bufferData->startIndex;

    //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                           //map the buffer to lock resource
        Vertex* pV = (Vertex*) resource.pData;                                                      //convert data to Vertex* so we can set
        memcpy(&pV[bufferData->startVertex], mesh.vertices, sizeof( Vertex ) * mesh.vertexCount);   //memcopy the vertices in
    context->Unmap(vertexBuffer, 0);                                                                //unmap to unlock resource
    bufferData->startVertex += mesh.vertexCount;                                                    //increment startIndex(vertices)


    //update index buffer
    context->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                                //lock resource
        unsigned int* pI = (unsigned int *)resource.pData;                                              //convert to unsigned int*
        memcpy(&pI[bufferData->startIndex], mesh.indices, sizeof( unsigned int ) * mesh.indexCount);    //copy indices in
    context->Unmap(indexBuffer, 0);                                                                     //unlock resource
    bufferData->startIndex += mesh.indexCount;                                                          //increment startIndex(indices)

    //tie mesh to buffer
    bufferToMeshes[mesh.bufferHandle.bufferID].push_back(mesh.ID);  

    return 0;
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

    
    bufferToMeshes.clear();
    buffers.clear();

    vertexBuffers.clear();
    indexBuffers.clear();

    IDGen_buffer = 0;
}