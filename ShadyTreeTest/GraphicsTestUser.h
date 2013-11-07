#pragma once

#include "GraphicsDevice.h"

class GraphicsTestUser
{
public:
    GraphicsTestUser(GraphicsDevice* gd);
    ~GraphicsTestUser(void);

    int init();

    int GraphicsTestUser::setTestTexture();
    int GraphicsTestUser::setWorldViewProj();
    int GraphicsTestUser::createVertexBuffers();
    int GraphicsTestUser::createShaders();

     void testdraw();

    GraphicsDevice* gd;

    TextureHandle t;
    BufferHandle bufH;
    MeshHandle m;
    VertexShaderHandle v;
    PixelShaderHandle p;
};

