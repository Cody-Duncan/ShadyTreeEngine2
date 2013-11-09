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
    MeshHandle m;
    VertexBufferHandle hVBuf;
    IndexBufferHandle hIBuf;
    
    VertexShaderHandle v;
    PixelShaderHandle p;
};

