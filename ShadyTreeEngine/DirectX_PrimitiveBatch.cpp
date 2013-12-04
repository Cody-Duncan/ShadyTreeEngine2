#include "DirectX_PrimitiveBatch.h"
#include "BufferResourcer.h"
#include "Resources.h"
#include "DirectX_GraphicsDevice.h"

#define BatchSize 2000

DirectX_PrimitiveBatch::DirectX_PrimitiveBatch(GraphicsDevice* deviceIn) : PrimitiveBatch(DeviceAPI::DirectX11)
{
    DirectX_GraphicsDevice* gDevice = dynamic_cast<DirectX_GraphicsDevice*>(deviceIn);
    if(!gDevice)
    {
        device = 0;
        DebugPrintf("Tried to use non-directX graphics device in directX spritebatch");
    }
    else
    {
        device = gDevice;
    }
}


DirectX_PrimitiveBatch::~DirectX_PrimitiveBatch(void)
{
    
}

void DirectX_PrimitiveBatch::Init()
{
    //color shaders
    colorVertSH   = Resources::Instance().LoadVertexShaderFile("resources/ColorShader.fx","VS", "vs_4_0");
                    Resources::Instance().VerifyVertexSize("resources/ColorShader.fx", sizeof(VertexCol));
    colorPixSH    = Resources::Instance().LoadPixelShaderFile("resources/ColorShader.fx","PS", "ps_4_0");

    //----- INITIALIZE TRIANGLE INDEX BUFFER
    BufferResourcer::Instance().createDynamicIndexBuffer(BatchSize*6, device->getDevice(), &triBatchIBuffer);
    unsigned int* indices = new unsigned int[BatchSize *6];
    for(int i = 0; i < BatchSize*6; ++i)
    {
        indices[i] = i;
    }

    ID3D11DeviceContext* context = device->getContext();
    ID3D11Buffer* TriIndexBuffer = BufferResourcer::Instance().getIBuffer(triBatchIBuffer).getIndexBuffer();

    //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(TriIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);     //map the buffer to lock resource
        unsigned int* pI = (unsigned int*) resource.pData;                                    //convert data to unisgned int* so it can be assigned
        DebugAssert(pI != 0, "Pointer to the dynamic index buffer is null. Error in allocating Buffer");
        memcpy(pI, indices, sizeof( unsigned int ) * BatchSize*6);              //memcopy the indices in
    context->Unmap(TriIndexBuffer, 0);                                          //unmap to unlock resource

     delete[] indices;

    //----- SET OTHER VALUES
    device->setClearColor(Color(0.4f,0.6f,0.9f,1.0f)); // cornflower blue

   
}

void DirectX_PrimitiveBatch::Dispose()
{
    triBatchVBuffers.clear();

    for(auto iter = triBatch.begin(); iter != triBatch.end(); ++iter)
    {
        iter->clear();
    }

    triBatch.clear();

    //@NOTE: This clears the capacity of spritebatch batches. May need to get that memory back later.
    //std::unordered_map<TextureHandle, std::vector<Vertex> >().swap(batch);

}

void DirectX_PrimitiveBatch::addTriBatchBuffer(unsigned int layer)
{
    if(layer >= triBatchVBuffers.size())
    {
        DebugAssert(device, "Spritebatch's graphicsDevice is null");

        VertexBufferHandle quadBuffer;
        BufferResourcer::Instance().createDynamicVertexBuffer(BatchSize*4, device->getDevice(), &quadBuffer);

        //assign vBuffer
        triBatchVBuffers.resize(layer);
        triBatchVBuffers[layer] = quadBuffer;

        //create a batch
        triBatch.resize(layer);
        triBatch[layer].reserve(BatchSize*4);
    }
}
void DirectX_PrimitiveBatch::resetTriBatchBuffer(unsigned int layer)
{
    VertexBufferData& triBufferData = BufferResourcer::Instance().getVBuffer(triBatchVBuffers[layer]);
    triBufferData.startVertex = 0;

    triBatch[layer].clear();
}
void DirectX_PrimitiveBatch::resetAllTriBatchBuffers()
{
    for(unsigned int i = 0; i < triBatch.size(); ++i)
    {
        resetTriBatchBuffer(i);
    }
}


void DirectX_PrimitiveBatch::DrawTriangles(unsigned int layer, Vector2* points, int pointLength, Matrix transform, Color c)
{
    //going to need to get a shader for drawing triangles.
    //need to reserve an index for a batch, and batchVBuffer.
    //need a mechanism for sorting the layering of the geometry, might use z position.

    std::vector<VertexCol>& batchRef = triBatch[layer];
    for(int i = 0; i < pointLength; ++i)
    {
        Vector2 temp = Vector2::Transform(points[i], transform);
        batchRef.push_back( VertexCol(Vector4(temp.x, temp.y, 0, 1), c) );
    }

    VertexBufferData& triBufferData = BufferResourcer::Instance().getVBuffer(triBatchVBuffers[layer]);
    triBufferData.startVertex += pointLength;
}


void DirectX_PrimitiveBatch::DrawBatch(int layer)
{
    VertexBufferData& quadBufferData = BufferResourcer::Instance().getVBuffer(triBatchVBuffers[layer]);
    if(quadBufferData.startVertex > 0) //don't draw empty buffers
    {
        DebugAssert(triBatchIBuffer.IbufferID >= 0, "Batch Index Buffer ID is invalid. Value: %d", triBatchIBuffer.IbufferID );
        //device->Draw(batchVBuffers[layer], triBatchIBuffer, t);
    }
}


void DirectX_PrimitiveBatch::sendTriBatchToBuffers(unsigned int layer)
{
    VertexBufferData& triBufferData = BufferResourcer::Instance().getVBuffer(triBatchVBuffers[layer]);
    std::vector<VertexCol>& currBatch = triBatch[layer];

    ID3D11DeviceContext* context = device->getContext();
    ID3D11Buffer* vertexBuffer = triBufferData.getVertexBuffer();

    //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                           //map the buffer to lock resource
        Vertex* pV = (Vertex*) resource.pData;                                                      //convert data to Vertex* so we can set
        DebugAssert(pV != 0, "Pointer to Dynamic Vertex Buffer is null. Cannot modify vertices. Check allocation of buffer.");
        DebugAssert(currBatch.size() <= BatchSize*4, "Too many vertices for buffer size. Crash now before we crash the graphics card. Tried to draw %d vertices", (unsigned int)currBatch.size());
        memcpy(pV, currBatch.data(), sizeof( Vertex ) * triBufferData.startVertex);   //memcopy the vertices in
    context->Unmap(vertexBuffer, 0);                                                                //unmap to unlock resource

    //set index buffer
    IndexBufferData& indexBufData = BufferResourcer::Instance().getIBuffer(triBatchIBuffer);
    indexBufData.startIndex = triBufferData.startVertex * 3 / 2; //6 indices per 4 vertices
}

void DirectX_PrimitiveBatch::Begin(bool alphaBlend)
{   
    static Matrix m = Matrix::Identity();
    device->setWorld(m);

    Vector4 eye(0.0f, 0.0f, 10.0f, 1.0f);
    Vector4 at(0.0f, 0.0f, 0.0f, 1.0f);
    Vector4 up(0.0f, 1.0f, 0.0f, 1.0f );
    device->setView( eye, at, up );

    device->setOrthographicProjection();
    
    device->setBlend(alphaBlend);

    device->setVertexShader(colorVertSH);
    device->setPixelShader(colorPixSH);

    device->setTextureSampler(false);
    device->ToggleDepthBuffer(false);

    device->BeginDraw();
}

void DirectX_PrimitiveBatch::End()
{
    for(unsigned int i = 0; i < triBatchVBuffers.size(); ++i)
    {
        sendTriBatchToBuffers(i);
        DrawBatch(i);
    }

    resetAllTriBatchBuffers();
}