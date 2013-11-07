#include "DirectX_GraphicsDevice.h"
#include "WindowFactory.h"          //ghMainWnd

#include <assert.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <sstream>
#include "Vertex.h"
#include "Debug_Graphics.h"
#include "MeshResourcer.h"
#include "Mesh.h"



DirectX_GraphicsDevice::DirectX_GraphicsDevice(void) : 
    driverType(D3D_DRIVER_TYPE_HARDWARE), 
    featureLevel(D3D_FEATURE_LEVEL_11_0),
    device(NULL),
    deviceContext(NULL),
    swapChain(NULL),
    renderTargetView(NULL),
    depthStencilView(NULL),
    depthStencilBuffer(NULL),
    Width(800), 
    Height(600), 
    enable4xMsaa(false),
    samplerLinear(nullptr),
    s_vMeshColor(Vector4(0.9f, 0.0f, 0.9f,1.0f)) //bright purple
{
    
}


DirectX_GraphicsDevice::~DirectX_GraphicsDevice(void)
{
}

void DirectX_GraphicsDevice::Free()
{
    //clear states
    if( deviceContext ) deviceContext->ClearState();

    ShaderResourcer::Instance().Dispose();
    BufferResourcer::Instance().Dispose();
    TextureResourcer::Instance().Dispose();

    if(samplerLinear) samplerLinear->Release();

    //release resources
    if( depthStencilBuffer ) depthStencilBuffer->Release();
    if( depthStencilView ) depthStencilView->Release();

    if( renderTargetView ) renderTargetView->Release();
    if( swapChain ) swapChain->Release();
    if( deviceContext ) deviceContext->Release();

    ReportLiveObjects(device);

    //debugDev->Release();
    if( device ) device->Release();
}

int DirectX_GraphicsDevice::Init()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( ghMainWnd, &rc );
    Width = rc.right - rc.left;
    Height = rc.bottom - rc.top;

    // Create the device and device deviceContext.
    unsigned int createDeviceFlags = 0;
    #if defined(DEBUG) || defined(_DEBUG)  
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDevice(
        0,                 // use default display adapter
        driverType,
        0,                 // no software device
        createDeviceFlags, 
        0, 0,              // default feature level array
        D3D11_SDK_VERSION,
        &device,
        &featureLevel,
        &deviceContext);

    DebugPrintf("GRAPHICS: Creating Graphics Device\n");

    if (errorBox(hr, L"D3D11CreateDevice Failed.")) return false;
    
    if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
    {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }

    hr = device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
    if (errorBox(hr, L"Error Checking Multi Sample Quality Levels")) return false;

    assert( m4xMsaaQuality > 0 );

    

    //======= Generate Swap Chain Description =============
    DXGI_SWAP_CHAIN_DESC sd;
    //describe backbuffer
    sd.BufferDesc.Width  = Width;
    sd.BufferDesc.Height = Height;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //backbuffer pixel format
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;  //use backbuffer as render target
    sd.BufferCount  = 1;                                //1 backbuffer, double buffering
    sd.OutputWindow = ghMainWnd;                        //give the window to render onto
    sd.Windowed     = true;                             //true = windowed, false = fullscreen
    sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;         //let the display driver select the most efficient presentation method
    sd.Flags        = 0;

    sd.SampleDesc.Count = enable4xMsaa ? 4 : 1;
    sd.SampleDesc.Quality = enable4xMsaa ? m4xMsaaQuality-1 : 0;

    //====== Create the Swap Chain ========
    {
        IDXGIDevice* dxgiDevice = 0;
        hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
        if ( errorBox(hr, L"Could not retrieve dxgiInterface.") ) return hr;

        IDXGIAdapter* dxgiAdapter = 0;
        hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
        if (errorBox(hr, L"Could not retrieve dxgiAdapter")) return hr;

        IDXGIFactory* dxgiFactory = 0;
        hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
        if (errorBox(hr, L"Could not retrieve dxgiFactory")) return hr;
    
        // Now, create the swap chain.
        hr = dxgiFactory->CreateSwapChain(device, &sd, &swapChain);
        if (errorBox(hr, L"Could not create SwapChain.")) return hr;
    
        // Release our acquired COM interfaces (because we are done with them).
        dxgiDevice->Release();
        dxgiAdapter->Release();
        dxgiFactory->Release();
    }


    if( hr = OnResize() )
    {
        return hr;
    }

    if( hr =  setTextureSampler() )
    {
        return hr;
    }

    return 0;
}

int DirectX_GraphicsDevice::OnResize()
{
    assert(deviceContext);
    assert(device);
    assert(swapChain);

    HRESULT hr = S_OK;
    
    //create RenderTargetView
    ID3D11Texture2D* backBuffer;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    errorBox(hr, L"Failed to get backbuffer from swap chain");
    device->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
    backBuffer->Release();

    //Create DepthStencilView
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = Width;
    depthStencilDesc.Height = Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    // Use 4X MSAA? --must match swap chain MSAA values.
    depthStencilDesc.SampleDesc.Count = enable4xMsaa ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = enable4xMsaa ? m4xMsaaQuality-1 : 0;
    

    HRESULT result = device->CreateTexture2D(
        &depthStencilDesc,		// Description of texture to create.
        0,
        &depthStencilBuffer		// Return pointer to depth/stencil buffer.
    );
    
    result = device->CreateDepthStencilView(
        depthStencilBuffer,		// Resource we want to create a view to.
        0,
        &depthStencilView		// Return depth/stencil view
    ); 

    // Bind Render Target and Depth stencil to output merger
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    //Create Viewport
    screenViewport.TopLeftX = 0.0f;
    screenViewport.TopLeftY = 0.0f;
    screenViewport.Width = static_cast<float>(Width);
    screenViewport.Height = static_cast<float>(Height);
    screenViewport.MinDepth = 0.0f;
    screenViewport.MaxDepth = 1.0f;

    deviceContext->RSSetViewports(1, &screenViewport);

    return 0;
}
int DirectX_GraphicsDevice::setTextureSampler()
{
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    return device->CreateSamplerState( &sampDesc, &samplerLinear );
}

void DirectX_GraphicsDevice::clearRenderTarget()
{
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectX_GraphicsDevice::BeginDraw()
{
    using namespace DirectX;
    assert(deviceContext);
    assert(swapChain);

    //set shaders and resources
    vertexShader = ShaderResourcer::Instance().getVertexShader(toDraw_vertexShader);
    pixelShader  = ShaderResourcer::Instance().getPixelShader(toDraw_pixelShader);
    inputLayout = ShaderResourcer::Instance().getInputLayout(toDraw_vertexShader);
    constantBuffer = ShaderResourcer::Instance().getConstantBuffer(toDraw_vertexShader);

    deviceContext->IASetInputLayout(inputLayout);
    deviceContext->VSSetShader( vertexShader, nullptr, 0 );
    deviceContext->PSSetShader( pixelShader, nullptr, 0 );

    //set it to the current vertexShader's infrequent buffer
    CBChangesInfrequently newProjBuffer = {g_Projection};
    deviceContext->UpdateSubresource( constantBuffer.infrequentBuffer, 0, nullptr, &newProjBuffer, 0, 0 );
    deviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer.infrequentBuffer );
    
    
}

void DirectX_GraphicsDevice::Draw(MeshHandle& hMesh, TextureHandle& hTex)
{
    //Setup the world/view matrices
    CBChangesEveryFrame cb_Frame;
    cb_Frame.mWorld = g_World;
    cb_Frame.mView = g_View;
    cb_Frame.vMeshColor = s_vMeshColor;

    //get mesh data;
    Mesh& mesh = (* MeshResourcer::Instance().getMesh(hMesh));
    BufferData buffer = BufferResourcer::Instance().getBuffer(mesh.bufferHandle);
    ID3D11Buffer* vertexBuffer = buffer.getVertexBuffer();
    ID3D11Buffer* indexBuffer = buffer.getIndexBuffer();
    unsigned int stride = buffer.stride;
    unsigned int vertexoffset = mesh.vertexOffset;
    unsigned int indexOffset = mesh.indexOffset;
    unsigned int vertexCount = buffer.vertexLength;
    unsigned int indexCount = buffer.indexLength;

    //get texture data
    ID3D11ShaderResourceView* texture = TextureResourcer::Instance().getTextureView(hTex);

    //input assembler for mesh data
    deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &vertexoffset );
    deviceContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    deviceContext->IASetPrimitiveTopology( buffer.primitiveTopology );

    //set constant buffer for world/view matrix
    deviceContext->UpdateSubresource( constantBuffer.frameBuffer, 0, nullptr, &cb_Frame, 0, 0 );
    deviceContext->VSSetConstantBuffers( 1, 1, &constantBuffer.frameBuffer );

    //set texture resources on pixel shader
    deviceContext->PSSetShaderResources( 0, 1, &texture );
    deviceContext->PSSetSamplers( 0, 1, &samplerLinear );
    deviceContext->PSSetConstantBuffers( 1, 1, &constantBuffer.frameBuffer );

    //draw
    deviceContext->DrawIndexed( indexCount, indexOffset, vertexoffset ); 
}
void DirectX_GraphicsDevice::EndDraw()
{

}

//======================= Public Methods ======================================
//=============================================================================
//=============================================================================

Mesh* DirectX_GraphicsDevice::generateMesh(std::string name)
{
    return MeshResourcer::Instance().generateMesh(name);
}

int DirectX_GraphicsDevice::createVertexIndexBuffer(Mesh* mesh, BufferHandle* handle)
{
    BufferResourcer& resourceVertexBuf = BufferResourcer::Instance();
    int result = resourceVertexBuf.createVertexIndexBuffer(*mesh, device, handle);
    if(result)
        return result;

    return 0;
}


void DirectX_GraphicsDevice::createTexture(std::string filename, TextureHandle* texHandle)
{
    TextureResourcer::Instance().createTextureFromWIC( device, deviceContext, filename.c_str(), texHandle);
}


int DirectX_GraphicsDevice::createVertexShader(std::string FileName, const char *EntryPoint, const char *ShaderModel, VertexShaderHandle* vsHandle)
{
    ShaderResourcer& SR = ShaderResourcer::Instance(); 
    HRESULT result = 0;
    result = SR.GenerateVertexShaderFromFile(
        device,
        deviceContext,
        FileName.c_str(),
        EntryPoint,
        ShaderModel);

    if ( errorBox(result, L" Could not generate vertex shader ") )  
        return result;
    
    *vsHandle = SR.getVertexShaderHandle(FileName);

    if( SR.debug_GetInputLayoutByteLength(*vsHandle) != sizeof(Vertex) )
    {
        std::wstringstream ss;
        ss << L" Vertex Size Differs from inputLayout. InputLayout:  " << SR.debug_GetInputLayoutByteLength(*vsHandle)
           << L" ; Vertex: " << sizeof(Vertex) << std::endl;
        errorBox(-1, ss.str().c_str());
        return 1;
    }

    return 0;
}


int DirectX_GraphicsDevice::createPixelShader(std::string FileName, const char *EntryPoint, const char *ShaderModel, PixelShaderHandle* psHandle)
{
    ShaderResourcer& SR = ShaderResourcer::Instance(); 
    int result = SR.GeneratePixelShaderFromFile(
        device,
        deviceContext,
        FileName.c_str(),
        EntryPoint,
        ShaderModel
        );

    if ( errorBox(result, L" Could not generate pixel shader") )  
        return result;

    *psHandle = SR.getPixelShaderHandle(FileName);

    return 0;
}



void DirectX_GraphicsDevice::setClearColor(Color color)
{
    clearColor[0] = color.x;
    clearColor[1] = color.y; 
    clearColor[2] = color.z;
    clearColor [3] = color.w;
}

void DirectX_GraphicsDevice::setVertexShader(VertexShaderHandle& hVertexShader)
{
    toDraw_vertexShader = hVertexShader;
}


void DirectX_GraphicsDevice::setPixelShader(PixelShaderHandle& hPixelShader)
{
    toDraw_pixelShader = hPixelShader;
}


void DirectX_GraphicsDevice::setWorld(Matrix& m)
{
    g_World = XMLoadFloat4x4(&m) ;
}


void DirectX_GraphicsDevice::setView(Vector4& eye, Vector4& at, Vector4& up)
{
    using namespace DirectX;

    g_Up = XMLoadFloat4( &up );
    updateView(eye, at);
}

void DirectX_GraphicsDevice::updateView(Vector4& eye, Vector4& at)
{
    using namespace DirectX;

    // Initialize the view matrix
    XMVECTOR Eye = XMLoadFloat4( &eye );
    XMVECTOR At = XMLoadFloat4( &at );
    g_View = XMMatrixLookAtLH( Eye, At, g_Up );
}


void DirectX_GraphicsDevice::setProjection(float fovAngleY, float nearClip, float farClip)
{
    g_Projection = DirectX::XMMatrixPerspectiveFovLH( fovAngleY, Width / (FLOAT)Height, nearClip, farClip);
}

void DirectX_GraphicsDevice::setOrthographicProjection(float nearClip, float farClip)
{
    g_Projection = DirectX::XMMatrixOrthographicLH((float)Width, (float)Height, nearClip, farClip);
}

void DirectX_GraphicsDevice::SwapBuffer()
{
    if(swapChain->Present(0, 0) < 0)
    {
        MessageBox(0, L"Swap Chain swapping broke.", 0, 0);
    }
}

