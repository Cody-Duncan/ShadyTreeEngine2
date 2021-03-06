#pragma once

#include "GraphicsDevice.h"
#include "SpriteBatch.h"
#include "GraphicsFactory.h"
#include "Stopwatch.h"

class SpriteBatchTestUser
{

public:
    SpriteBatchTestUser(GraphicsDevice* in) : gd(in) {}
    ~SpriteBatchTestUser(void);

    int init();

    int setTestTexture();

    void testdraw();

    GraphicsDevice* gd;
    SpriteBatch* sb;

    TextureHandle t;
    MeshHandle m;
    VertexShaderHandle v;
    PixelShaderHandle p;

    Matrix* matrixArray;
    int matrixArrayLen;

    Stopwatch watch;
};

