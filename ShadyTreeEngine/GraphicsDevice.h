#pragma once

#include "MathLib.h"
#include "GraphicsResourceHandles.h"
#include <string>
#include "Vertex.h"
#include "Shady_DLLExport.h"

class ST_API GraphicsDevice
{
public:
    GraphicsDevice(void);
    ~GraphicsDevice(void);

    virtual int Init() = 0;
    virtual int OnResize() = 0;
    virtual void BeginDraw() = 0;
    virtual void Draw(MeshHandle& hMesh, TextureHandle& hTex) = 0;
    virtual void EndDraw() = 0;
    virtual void PresentFrame() = 0;
    virtual void Free() = 0;

    virtual int createMesh(std::string name, Vertex* vertices, int vertexCount, unsigned int* indices, int numIndices, MeshHandle* handle) = 0;
    virtual void createTexture(std::string filename, TextureHandle* texHandle) = 0;
    virtual int createVertexShader(std::string FileName, const char *EntryPoint, const char *ShaderModel, VertexShaderHandle* vsHandle) = 0;
    virtual int createPixelShader(std::string FileName, const char *EntryPoint, const char *ShaderModel, PixelShaderHandle* psHandle) = 0;

    virtual void setClearColor(Color color) = 0;
    virtual void clearRenderTarget() = 0;

    virtual void setVertexShader(VertexShaderHandle&) = 0;
    virtual void setPixelShader(PixelShaderHandle&) = 0;

    virtual void setWorld(Matrix&) = 0;
    virtual void setView(Vector4& eye, Vector4& at, Vector4& up) = 0;
    virtual void updateView(Vector4& eye, Vector4& at) = 0;
    virtual void setProjection(float fovAngleY = 1.570796371f, float nearClip = 0.01f, float farClip = 1000.0f) = 0;
    virtual void setOrthographicProjection(float nearClip = 0.01f, float farClip = 1000.0f) =0;
    

    
};

