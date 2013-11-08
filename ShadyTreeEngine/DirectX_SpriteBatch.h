#pragma once

#include "SpriteBatch.h"
#include "GraphicsDevice.h"
#include "DirectX_GraphicsDevice.h"



class DirectX_SpriteBatch : public SpriteBatch
{
public:
    DirectX_SpriteBatch(GraphicsDevice* device);
    ~DirectX_SpriteBatch(void);

    void Init();
    void createTexture(std::string Filename, TextureHandle* texHandle);

    void Begin();
    void Draw(TextureHandle texH, Matrix transform, Rectangle2 rect);
    void End();

    void DrawBatch(TextureHandle t);

private:
    DirectX_GraphicsDevice* device;

    int vertexLength;
    int indexLength;


    TextureHandle texH;
    VertexShaderHandle vertexShaderH;
    PixelShaderHandle pixelShaderH;

    void addBatchBuffer(TextureHandle t);
    void resetBatchBuffer(TextureHandle t);
    void resetAllBatchBuffers();

    void sentBatchToBuffers();

    //textureID -> (transform, textureArea)
    //std::unordered_map<TextureHandle, std::vector< std::pair<Matrix, Rectangle2 > > > batchBuffer;
    std::unordered_map<TextureHandle, BufferHandle> batchBuffer;
    std::unordered_map<TextureHandle, std::pair< std::vector<Vertex>, std::vector<unsigned int> > > batch;
};

