#pragma once

#include "SpriteBatch.h"
#include "GraphicsDevice.h"
#include "DirectX_GraphicsDevice.h"

struct FontLetter
{
    int id;
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
};

class FontTexture
{
public:
    TextureHandle t;
    std::unordered_map<int, FontLetter> letters;

    std::string face;
    std::string textureFileName;
};

class DirectX_SpriteBatch : public SpriteBatch
{
public:
    DirectX_SpriteBatch(GraphicsDevice* device);
    ~DirectX_SpriteBatch(void);

    void Init();

    void Begin(bool AlphaBlend = false);
    void Draw(TextureHandle texH, Matrix transform, Rectangle2 rect);
    void End();

    void Dispose(); 

    void DrawBatch(TextureHandle t);

    void LoadDebugFont(std::string filename);

private:
    DirectX_GraphicsDevice* device;

    int vertexLength;
    int indexLength;

    VertexShaderHandle vertexShaderH;
    PixelShaderHandle pixelShaderH;

    void addBatchBuffer(TextureHandle t);
    void resetBatchBuffer(TextureHandle t);
    void resetAllBatchBuffers();

    void sentBatchToBuffers(TextureHandle t);

    //textureID -> (transform, textureArea)
    //std::unordered_map<TextureHandle, std::vector< std::pair<Matrix, Rectangle2 > > > batchBuffer;
    std::unordered_map<TextureHandle, VertexBufferHandle> batchVBuffers;
    IndexBufferHandle batchIBuffer;
    std::unordered_map<TextureHandle, std::vector<Vertex> > batch;

    FontTexture fontTex;
};

