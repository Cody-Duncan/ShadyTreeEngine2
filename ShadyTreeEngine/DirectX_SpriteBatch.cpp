#include "stdafx.h"
#include "DirectX_SpriteBatch.h"
#include "BufferResourcer.h"

#define BatchSize 250

DirectX_SpriteBatch::DirectX_SpriteBatch(GraphicsDevice* deviceIn) : SpriteBatch(DeviceAPI::DirectX11)
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


DirectX_SpriteBatch::~DirectX_SpriteBatch(void)
{
}



void DirectX_SpriteBatch::Init()
{
    device->createVertexShader("Tutorial07.fx","VS", "vs_4_0", &vertexShaderH);
    device->createPixelShader("Tutorial07.fx","PS", "ps_4_0", &pixelShaderH);

    BufferResourcer::Instance().createDynamicIndexBuffer(BatchSize*6, device->getDevice(), &batchIBuffer);

    unsigned int* indices = new unsigned int[BatchSize *6];
    int v = 0;
    for(int i = 0; i < BatchSize*6; (i+=6, v+=4))
    {
        indices[i] = v;
        indices[i+1] = v+1;
        indices[i+2] = v+2;

        indices[i+3] = v;
        indices[i+4] = v+2;
        indices[i+5] = v+3;
    }

    ID3D11DeviceContext* context = device->getContext();
    IndexBufferData& IndexBufferData = BufferResourcer::Instance().getIBuffer(batchIBuffer);
    ID3D11Buffer* indexBuffer = IndexBufferData.getIndexBuffer();

     //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);    //map the buffer to lock resource
        unsigned int* pI = (unsigned int*) resource.pData;                  //convert data to Vertex* so we can set
        assert(pI != 0);
        memcpy(pI, indices, sizeof( unsigned int ) * BatchSize*6);          //memcopy the vertices in
    context->Unmap(indexBuffer, 0);                                         //unmap to unlock resource
}

void DirectX_SpriteBatch::addBatchBuffer(TextureHandle t)
{
    if(batchVBuffers.find(t) == batchVBuffers.end()) //if key nonexistent
    {
        assert(device && "Spritebatch does not have a valid graphicsDevice");
    
        VertexBufferHandle quadBuffer;
        BufferResourcer::Instance().createDynamicVertexBuffer(BatchSize*4, device->getDevice(), &quadBuffer);

        batchVBuffers[t] = quadBuffer;
    }
}

void DirectX_SpriteBatch::resetBatchBuffer(TextureHandle t)
{
    VertexBufferData& quadBufferData = BufferResourcer::Instance().getVBuffer(batchVBuffers[t]);
    quadBufferData.startVertex = 0;

    batch[t].clear();
}

void DirectX_SpriteBatch::resetAllBatchBuffers()
{
    for(auto iter = batchVBuffers.begin(); iter != batchVBuffers.end(); iter++)
    {
        resetBatchBuffer(iter->first);
    }
}

void DirectX_SpriteBatch::createTexture(std::string Filename, TextureHandle* texHandle)
{
    device->createTexture(Filename, texHandle);
}

void DirectX_SpriteBatch::Begin()
{   
    static Matrix m = Matrix::Identity();
    device->setWorld(m);

    Vector4 eye(0.0f, 0.0f, 10.0f, 1.0f);
    Vector4 at(0.0f, 0.0f, 0.0f, 1.0f);
    Vector4 up(0.0f, 1.0f, 0.0f, 1.0f );
    device->setView( eye, at, up );

    device->setOrthographicProjection();

    device->setClearColor(Color(0.4f,0.6f,0.9f,1.0f));
    device->clearRenderTarget();
    
    //device->ToggleDepthBuffer(false);

    device->setVertexShader(vertexShaderH);
    device->setPixelShader(pixelShaderH);

    device->setTextureSampler(false);

    device->BeginDraw();
}


void DirectX_SpriteBatch::Draw(TextureHandle texH, Matrix transform, Rectangle2 rect)
{
    addBatchBuffer(texH);
    TextureData texData =  TextureResourcer::Instance().getTextureData(texH);
    
    //calculate normalized UV coordinates
    Rectangle2 textureArea;
    textureArea.position = Vector2(rect.position.x / texData.width, rect.position.y / texData.width);
    textureArea.dimensions = Vector2(rect.dimensions.x / texData.width, rect.dimensions.y / texData.width);

    //calculate mesh vertex positions
    Vector2 position = Vector2(0,0);
    Vector2 corner = rect.dimensions;
    position = Vector2::Transform(position, transform);
    corner = Vector2::Transform(corner, transform);

    VertexBufferData& quadBufferData = BufferResourcer::Instance().getVBuffer(batchVBuffers[texH]);
    
    //generate vertices and indices for quads
    //vertices need normalized UV coordinates based on the rect in texture Coordinates.

//#define CLOCKWISE_VERTS_SPRITEBATCH//
#define COUNTERCLOCKWISE_VERTS_SPRITEBATCH
#ifdef  COUNTERCLOCKWISE_VERTS_SPRITEBATCH
    //counter-clockwise
    Vertex vertices[] = 
    {
        { Vector4(position.x, corner.y, 0, 1), textureArea.botLeft() },    //0 topLeft
        { Vector4(position.x, position.y, 0, 1), textureArea.topLeft() },  //3 botLeft
        { Vector4(corner.x, position.y, 0, 1), textureArea.topRight() },   //2 botRight
        { Vector4(corner.x, corner.y, 0, 1), textureArea.botRight() },     //1 topRight
    };
#endif
#ifdef CLOCKWISE_VERTS_SPRITEBATCH
    //clockwise
    Vertex vertices[] = 
    {
        { Vector4(position.x, corner.y, 0, 1), textureArea.botLeft() },    //0 topLeft
        { Vector4(corner.x, corner.y, 0, 1), textureArea.botRight() },     //1 topRight
        { Vector4(corner.x, position.y, 0, 1), textureArea.topRight() },   //2 botRight
        { Vector4(position.x, position.y, 0, 1), textureArea.topLeft() },  //3 botLeft
    };
#endif

    //copy to end of batch
    std::vector<Vertex>& batchRef = batch[texH];
    batchRef.push_back(vertices[0]);
    batchRef.push_back(vertices[1]);
    batchRef.push_back(vertices[2]);
    batchRef.push_back(vertices[3]);

    quadBufferData.startVertex += 4;
}

void DirectX_SpriteBatch::End()
{
    for(auto iter = batchVBuffers.begin(); iter != batchVBuffers.end(); iter++)
    {
        sentBatchToBuffers(iter->first);
        DrawBatch(iter->first);
    }

    device->SwapBuffer();

    resetAllBatchBuffers();
}

void DirectX_SpriteBatch::DrawBatch(TextureHandle t)
{
    VertexBufferData& quadBufferData = BufferResourcer::Instance().getVBuffer(batchVBuffers[t]);
    if(quadBufferData.startVertex > 0) //don't draw empty buffers
    {
        assert(batchIBuffer.IbufferID >= 0);
        DebugPrintf("MarkDraw\n");
        device->Draw(batchVBuffers[t], batchIBuffer, t);
    }
}

void DirectX_SpriteBatch::sentBatchToBuffers(TextureHandle t)
{
    VertexBufferData& quadBufferData = BufferResourcer::Instance().getVBuffer(batchVBuffers[t]);
    std::vector<Vertex>& currBatch = batch[t];
    

    //NOTE: try batching the vertices and indices, 
    //then putting them in the vertex/index buffer all at once

    //get context and buffers
    ID3D11DeviceContext* context = device->getContext();
    ID3D11Buffer* vertexBuffer = quadBufferData.getVertexBuffer();
    

    //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                           //map the buffer to lock resource
        Vertex* pV = (Vertex*) resource.pData;                                                      //convert data to Vertex* so we can set
        assert(pV != 0);
        assert(currBatch.size() <= BatchSize*4 && "Too many vertices for buffer size. Crash now before we crash the graphics card.");
        memcpy(pV, currBatch.data(), sizeof( Vertex ) * quadBufferData.startVertex);   //memcopy the vertices in
    context->Unmap(vertexBuffer, 0);                                                                //unmap to unlock resource

    //set index buffer
    IndexBufferData& indexBufData = BufferResourcer::Instance().getIBuffer(batchIBuffer);
    indexBufData.startIndex = quadBufferData.startVertex * 3 / 2; //6 indices per 4 vertices
}