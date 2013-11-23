#include "ShaderResourcer.h"
#include <d3dcompiler.h>
#include "StringUtility.h"

int ShaderResourcer::IDGen_vs = 0;
int ShaderResourcer::IDGen_ps = 0;

HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    #if defined( DEBUG ) || defined( _DEBUG )
        dwShaderFlags |= D3DCOMPILE_DEBUG;
    #endif

    ID3DBlob* pErrorBlob = nullptr ;
    hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
    if( FAILED(hr) )
    {
        if( pErrorBlob != nullptr )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) 
        pErrorBlob->Release();

    return S_OK;
}


int CompileShader(const char* FileName, const char * EntryPoint, const char *ShaderModel, ID3DBlob** shaderBlob)
{
    WCHAR* filenameW = convertMultiByteToWCHAR(FileName);

    HRESULT hr = CompileShaderFromFile( filenameW, EntryPoint, ShaderModel, shaderBlob );

    delete[] filenameW;
    if(hr)
        return hr;
    return 0;
}

HRESULT CreateInputLayoutDescFromVertexShaderSignature( ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout, int* inputLayoutByteLength )
{
    // Reflect shader info
    ID3D11ShaderReflection* pVertexShaderReflection = nullptr; 
    HRESULT hr = S_OK;
    if ( FAILED( D3DReflect( pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pVertexShaderReflection ) ) ) 
    {
        return S_FALSE;
    }
     
    // get shader description
    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc( &shaderDesc );
     
    // Read input layout description from shader info
    unsigned int byteOffset = 0;
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for ( unsigned int i=0; i< shaderDesc.InputParameters; ++i )
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;       
        pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc );
 
        // create input element desc
        D3D11_INPUT_ELEMENT_DESC elementDesc;   
        elementDesc.SemanticName = paramDesc.SemanticName;      
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = byteOffset;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;   
 
        // determine DXGI format
        if ( paramDesc.Mask == 1 )
        {
            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
            byteOffset += 4;
        }
        else if ( paramDesc.Mask <= 3 )
        {
            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
            byteOffset += 8;
        }
        else if ( paramDesc.Mask <= 7 )
        {
            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            byteOffset += 12;
        }
        else if ( paramDesc.Mask <= 15 )
        {
            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            byteOffset += 16;
        }
             
        //save element desc
        inputLayoutDesc.push_back(elementDesc);
    }       
 
    // Try to create Input Layout
    hr = pD3DDevice->CreateInputLayout( &inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout );
 
    //Free allocation shader reflection memory
    pVertexShaderReflection->Release();

    //record byte length
    *inputLayoutByteLength = byteOffset;

    return hr;
}

//Backup InputLayout Resources, for testing
D3D11_INPUT_ELEMENT_DESC layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
unsigned int layoutNum = ARRAYSIZE(layout);

int generateInputLayout(const char* FileName, D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc, unsigned int numElementsLayout, ID3D11Device* device, ID3DBlob* compiledShader, ID3D11InputLayout** output)
{
    ID3D11InputLayout* vertexLayout = nullptr;
    HRESULT hr = device->CreateInputLayout( 
    inputLayoutDesc, numElementsLayout, 
    compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 
    &vertexLayout );

    if( FAILED( hr ) )
    {
        return hr;
    }

    *output = vertexLayout;

    return hr;
}
// end inputlayout resources


ShaderResourcer& ShaderResourcer::Instance()
{
    static ShaderResourcer  instance; 
    return instance;
}


int ShaderResourcer::GenerateVertexShaderFromFile(
    ID3D11Device* device, ID3D11DeviceContext* deviceContext,  
    const char* FileName, const char *EntryPoint, const char *ShaderModel)
{
    HRESULT hr = S_OK;

    //compile the shader
    ID3DBlob* shaderBlob = nullptr;
    if ( hr = CompileShader(FileName, EntryPoint, ShaderModel, &shaderBlob) )
        return hr;

    //Create the Vertex Shader
    ID3D11VertexShader* newVertexShader = 0;
    hr = device->CreateVertexShader( shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &newVertexShader );
    if( FAILED( hr ) )
    {	
        shaderBlob->Release();
        return hr;
    }

    
    //Create via reflection
    //Create Input Layout 
    ID3D11InputLayout* vertexLayout = nullptr;
    int vertexByteLength = 0;
    hr = CreateInputLayoutDescFromVertexShaderSignature(shaderBlob, device, &vertexLayout, &vertexByteLength);
    if( FAILED(hr) )
    {
        shaderBlob->Release();
        return hr;
    }


    //Create Constant Buffers
    ID3D11Buffer* infreqBuffer;
    ID3D11Buffer* frameBuffer;
    hr = GenerateConstantBuffers(device, &infreqBuffer, &frameBuffer);
    if( FAILED(hr) )
    {
        vertexLayout->Release();
        shaderBlob->Release();
        return hr;
    }

    shaderBlob->Release();

    int id = generateVertexShaderID();

    vertexShaderMap[id] = newVertexShader;
    inputLayouts[id] = vertexLayout;
    inputLayoutByteLength[id] = vertexByteLength;
    constantBuffers[id] = ConstantBufferPair(infreqBuffer, frameBuffer);

    nameToVertexShaderID[FileName] = id;

    return hr;
}

int ShaderResourcer::GeneratePixelShaderFromFile(
    ID3D11Device* device, ID3D11DeviceContext* deviceContext,  
    const char* FileName, const char *EntryPoint, const char *ShaderModel)
{
    HRESULT hr;

    //compile the shader
    ID3DBlob* shaderBlob = nullptr;
    if ( hr = CompileShader(FileName, EntryPoint, ShaderModel, &shaderBlob) )
        return hr;

    //create the pixel shader
    ID3D11PixelShader* newPixelShader = 0;
    hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &newPixelShader );
    shaderBlob->Release();
    if( FAILED( hr ) )
    {	
        return hr;
    }

    int id = generatePixelShaderID();
    pixelShaderMap[id] = newPixelShader;

    nameToPixelShaderID[FileName] = id;

    return hr;
}

int ShaderResourcer::GenerateConstantBuffers(
    ID3D11Device* device, 
    ID3D11Buffer** ppInfrequentBuffer, ID3D11Buffer** ppPerFrameBuffer)
{
    // Create the constant buffer
    ID3D11Buffer* tempInfreqBuffer;
    ID3D11Buffer* tempFrameBuffer;
    
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBChangesInfrequently);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = device->CreateBuffer( &bd, nullptr, &tempInfreqBuffer );

    if( FAILED( hr ) )
        return hr;

    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = device->CreateBuffer( &bd, nullptr, &tempFrameBuffer );

    if( FAILED( hr ) )
        return hr;

    *ppInfrequentBuffer = tempInfreqBuffer;
    *ppPerFrameBuffer = tempFrameBuffer;

    return 0;
}

 ConstantBufferPair* ShaderResourcer::getAllConstantBuffers(int& size)
{
    ConstantBufferPair* array = new ConstantBufferPair[constantBuffers.size()];
    int i = 0;
    for( auto iter = constantBuffers.begin(); iter != constantBuffers.end(); iter++, i++)
    {
        array[i] = (iter->second);
    }
    size = constantBuffers.size();
    return array;
}

//Handles by filename
VertexShaderHandle ShaderResourcer::getVertexShaderHandle(std::string shaderFileName)
{
    VertexShaderHandle v = { nameToVertexShaderID[shaderFileName] };
    return v;
}
PixelShaderHandle ShaderResourcer::getPixelShaderHandle(std::string shaderFileName)
{
    PixelShaderHandle p = {nameToPixelShaderID[shaderFileName]};
    return p;
}

//By Filename
ID3D11VertexShader* ShaderResourcer::getVertexShaderByFile(std::string shaderFileName)
{
    return vertexShaderMap[nameToVertexShaderID[shaderFileName]];
}

ID3D11PixelShader* ShaderResourcer::getPixelShaderByFile(std::string shaderFileName)
{
    return pixelShaderMap[nameToPixelShaderID[shaderFileName]];
}

ID3D11InputLayout* ShaderResourcer::getInputLayoutByFile(std::string shaderFileName)
{
    return inputLayouts[nameToVertexShaderID[shaderFileName]];
}

ConstantBufferPair ShaderResourcer::getConstantBufferByFile(std::string shaderFileName)
{
    return constantBuffers[nameToVertexShaderID[shaderFileName]];
}


//By ID
ID3D11VertexShader* ShaderResourcer::getVertexShader(VertexShaderHandle handle)
{
    return vertexShaderMap[handle.vertexShaderID];
}

ID3D11InputLayout* ShaderResourcer::getInputLayout(VertexShaderHandle handle)
{
    return inputLayouts[handle.vertexShaderID];
}

ID3D11PixelShader* ShaderResourcer::getPixelShader(PixelShaderHandle handle)
{
    return pixelShaderMap[handle.pixelShadeeID];
}

ConstantBufferPair ShaderResourcer::getConstantBuffer(VertexShaderHandle handle)
{
    return constantBuffers[handle.vertexShaderID];
}



int ShaderResourcer::generateVertexShaderID()
{
    return IDGen_vs++;
}
int ShaderResourcer::generatePixelShaderID()
{
    return IDGen_ps++;
}

 void ShaderResourcer::Dispose()
{
    for( auto iter = vertexShaderMap.begin(); iter != vertexShaderMap.end(); iter++)
    {
        iter->second->Release();
    }
    

    for( auto iter = pixelShaderMap.begin(); iter != pixelShaderMap.end(); iter++)
    {
        iter->second->Release();
    }
    

    for( auto iter = inputLayouts.begin(); iter != inputLayouts.end(); iter++)
    {
        iter->second->Release();
    }
    
    for( auto iter = constantBuffers.begin(); iter != constantBuffers.end(); iter++)
    {
        iter->second.Dispose();
    }

    constantBuffers.clear();
    inputLayouts.clear();
    inputLayoutByteLength.clear();
    pixelShaderMap.clear();
    vertexShaderMap.clear();

    nameToPixelShaderID.clear();
    nameToVertexShaderID.clear();

    IDGen_vs = 0;
    IDGen_ps = 0;

}

int ShaderResourcer::debug_GetInputLayoutByteLength(VertexShaderHandle vHandle)
{
    return inputLayoutByteLength[vHandle.vertexShaderID];
}