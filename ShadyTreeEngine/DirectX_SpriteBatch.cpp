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

}

void DirectX_SpriteBatch::addBatchBuffer(TextureHandle t)
{
    if(batchBuffer.find(t) == batchBuffer.end()) //if key nonexistent
    {
        assert(device && "Spritebatch does not have a valid graphicsDevice");
    
        BufferHandle quadBuffer;
        BufferResourcer::Instance().createDynamicVertexIndexBuffer(BatchSize*4, BatchSize*6, device->getDevice(), &quadBuffer);

        batchBuffer[t] = quadBuffer;
    }
}

void DirectX_SpriteBatch::resetBatchBuffer(TextureHandle t)
{
    BufferData& quadBufferData = BufferResourcer::Instance().getBuffer(batchBuffer[t]);
    quadBufferData.startVertex = 0;
    quadBufferData.startIndex = 0;
}

void DirectX_SpriteBatch::resetAllBatchBuffers()
{
    for(auto iter = batchBuffer.begin(); iter != batchBuffer.end(); iter++)
    {
         BufferData& quadBufferData = BufferResourcer::Instance().getBuffer(iter->second);
         quadBufferData.startVertex = 0;
         quadBufferData.startIndex = 0;
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

    Vector4 eye( 0.0f, 0.0f, 100.0f, 0.0f);
    Vector4 at(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 up(0.0f, 1.0f, 0.0f, 0.0f );
    device->setView( eye, at, up );

    device->setOrthographicProjection();

    device->setClearColor(Color(0.4f,0.6f,0.9f,1.0f));
    device->clearRenderTarget();

    device->setVertexShader(vertexShaderH);
    device->setPixelShader(pixelShaderH);

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

    BufferData& quadBufferData = BufferResourcer::Instance().getBuffer(batchBuffer[texH]);
    
    //generate vertices and indices for quads
    //vertices need normalized UV coordinates based on the rect in texture Coordinates.
    Vertex vertices[] = 
    {
        { Vector4(position.x, position.y, 0, 1), textureArea.topLeft() },  //0 topLeft
        { Vector4(corner.x, position.y, 0, 1), textureArea.topRight() },   //1 topRight
        { Vector4(corner.x, corner.y, 0, 1), textureArea.botRight() },     //2 botRight
        { Vector4(position.x, corner.y, 0, 1), textureArea.botLeft() },    //3 botLeft
    };

    //generate indices
    int startIndex = quadBufferData.startIndex;
    unsigned int indices[] = 
    {
        startIndex,
        startIndex + 1,
        startIndex + 2,
        startIndex,
        startIndex + 2,
        startIndex + 3
    };

    
    
}

void DirectX_SpriteBatch::End()
{
    for(auto iter = batchBuffer.begin(); iter != batchBuffer.end(); iter++)
    {
        DrawBatch(iter->first);
    }

    device->SwapBuffer();

    resetAllBatchBuffers();
}

void DirectX_SpriteBatch::DrawBatch(TextureHandle t)
{
    BufferData& quadBufferData = BufferResourcer::Instance().getBuffer(batchBuffer[t]);
    if(quadBufferData.startIndex > 0) //don't draw empty buffers
    {
        device->Draw(batchBuffer[t], t);
    }
}

void DirectX_SpriteBatch::sentBatchToBuffers(TextureHandle t)
{
    BufferData& quadBufferData = BufferResourcer::Instance().getBuffer(batchBuffer[texH]);

    //NOTE: try batching the vertices and indices, 
    //then putting them in the vertex/index buffer all at once

    //get context and buffers
    ID3D11DeviceContext* context = device->getContext();
    ID3D11Buffer* vertexBuffer = quadBufferData.getVertexBuffer();
    ID3D11Buffer* indexBuffer = quadBufferData.getIndexBuffer();

    //update vertex buffer
    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                           //map the buffer to lock resource
        Vertex* pV = (Vertex*) resource.pData;                                                      //convert data to Vertex* so we can set
        assert(pV != 0);
        memcpy(pV, 
        pV[quadBufferData.startVertex] = vertices[0];
        pV[quadBufferData.startVertex+1] = vertices[1];
        pV[quadBufferData.startVertex+2] = vertices[2];
        pV[quadBufferData.startVertex+3] = vertices[3];
        //memcpy(&pV[quadBufferData.startVertex], vertices, sizeof( Vertex ) * 4);                    //memcopy the vertices in
    context->Unmap(vertexBuffer, 0);                                                                //unmap to unlock resource
    quadBufferData.startVertex += 4;                                                                //increment startIndex(vertices)


    //update index buffer
    context->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);                            //lock resource
        unsigned int* pI = (unsigned int *)resource.pData;                                          //convert to unsigned int*
         assert(pI != 0);
         pI[quadBufferData.startIndex] = indices[0];
         pI[quadBufferData.startIndex+1] = indices[1];
         pI[quadBufferData.startIndex+2] = indices[2];
         pI[quadBufferData.startIndex+3] = indices[3];
         pI[quadBufferData.startIndex+4] = indices[4];
         pI[quadBufferData.startIndex+5] = indices[5];
        memcpy(&pI[quadBufferData.startIndex], indices, sizeof( unsigned int ) * 6);                //copy indices in
    context->Unmap(indexBuffer, 0);                                                                 //unlock resource
    quadBufferData.startIndex += 6;                                                                 //increment startIndex(indices)
}