#pragma once

#include "graphicsdevice.h"


#include "BufferResourcer.h"
#include "ConstantBuffer.h"
#include "TextureResourcer.h"
#include "ShaderResourcer.h"
#include "GraphicsResourceHandles.h"

#include "Mesh.h"

__declspec(align(16)) class DirectX_GraphicsDevice : public GraphicsDevice
{
private:
    DirectX::XMMATRIX    g_World;
    DirectX::XMMATRIX    g_View;
    DirectX::XMMATRIX    g_Projection;
    DirectX::XMVECTOR    g_Up;

public:
    DirectX_GraphicsDevice(void);
    ~DirectX_GraphicsDevice(void);

    virtual int Init();
    virtual int OnResize();
    virtual void clearRenderTarget();
    virtual void BeginDraw();
    virtual void Draw(MeshHandle& hMesh, TextureHandle& hTex);
    virtual void EndDraw();
    virtual void SwapBuffer();
    virtual void Free();

    virtual Mesh* generateMesh(std::string name);
    virtual int  createVertexIndexBuffer(Mesh* mesh, VertexBufferHandle* hVBuf, IndexBufferHandle* hIBuf);
    virtual void createTexture(std::string Filename, TextureHandle* texHandle);
    virtual int  createVertexShader(std::string FileName, const char *EntryPoint, const char *ShaderModel, VertexShaderHandle* vsHandle);
    virtual int  createPixelShader(std::string FileName, const char *EntryPoint, const char *ShaderModel, PixelShaderHandle* psHandle);

    virtual void setClearColor(Color color);

    virtual void setVertexShader(VertexShaderHandle&);
    virtual void setPixelShader(PixelShaderHandle&);

    virtual void setWorld(Matrix&);
    virtual void setView(Vector4& eye, Vector4& at, Vector4& up);
    virtual void updateView(Vector4& eye, Vector4& at);
    virtual void setProjection(float fovAngleY = 1.570796371f, float nearClip = 0.01f, float farClip = 1000.0f);
    virtual void setOrthographicProjection(float nearClip = 0.01f, float farClip = 1000.0f);


    ID3D11Device* getDevice();
    ID3D11DeviceContext* getContext();
    void Draw(VertexBufferHandle& hVBuf, IndexBufferHandle& hIBuf, const TextureHandle& hTex);
    int getWidth();
    int getHeight();

    void ToggleDepthBuffer(bool turnItOn);

protected:
    D3D_DRIVER_TYPE			driverType;
    D3D_FEATURE_LEVEL		featureLevel;
    ID3D11Device*			device;
    ID3D11DeviceContext*	deviceContext; 
    IDXGISwapChain*			swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11Texture2D*		depthStencilBuffer;
    D3D11_VIEWPORT			screenViewport;
    ID3D11SamplerState*     samplerLinear;

    ID3D11DepthStencilState* m_depthDisabledStencilState;
    ID3D11DepthStencilState* m_depthStencilState;

    int setTextureSampler();


    unsigned int m4xMsaaQuality;
    bool enable4xMsaa;
    int Width;
    int Height;
    float clearColor[4];
    DirectX::XMFLOAT4 s_vMeshColor;

    VertexShaderHandle toDraw_vertexShader;
    PixelShaderHandle toDraw_pixelShader;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;
    ConstantBufferPair constantBuffer;
};

