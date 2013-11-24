#include "stdafx.h"
#include "Resources.h"

#include "Mesh.h"
#include "MeshResourcer.h"
#include "BufferResourcer.h"
#include "FileResourcer.h"

IResources& Resources::Instance()
{
    static Resources res;
    return res;
}

Resources::Resources(void)
{
}


Resources::~Resources(void)
{
}

void Resources::setGraphicsDevice(GraphicsDevice* _gd)
{
    gd = dynamic_cast<DirectX_GraphicsDevice*>(_gd);
}

Mesh* Resources::generateMesh(std::string identifierName)
{
    return MeshResourcer::Instance().generateMesh(identifierName);
}

MeshHandle Resources::LoadMeshFile(std::string filename)
{
    assert(false && "NOT IMPLEMENTED YET: Loading Mesh from file");
    MeshHandle h = {-1};
    return h;
}

MeshHandle Resources::BindMesh(Mesh* mesh)
{
    VertexBufferHandle v;
    IndexBufferHandle i;

    int result =  BufferResourcer::Instance().createStaticBuffers(*mesh, gd->getDevice(), &v, &i);
    assert(result == 0);

    return mesh->handle();
}


TextureHandle Resources::LoadTextureFile(std::string token, std::string filename)
{
    TextureHandle texHandle;
    const char* filenameCSTR = filename.c_str();
    int result = TextureResourcer::Instance().createTextureFromWIC(gd->getDevice(), gd->getContext(), filenameCSTR, &texHandle);
    if(!result)
    {
        tokenToTexH[token] = texHandle;
        return texHandle;
    }
    return texHandle;
}

TextureHandle Resources::LoadTexture(std::string token, uint8_t* data, int length)
{
    TextureHandle texHandle;
    int result = TextureResourcer::Instance().createTextureFromWIC(gd->getDevice(), gd->getContext(), data, length, &texHandle);
    if(!result)
    {
        tokenToTexH[token] = texHandle;
        return texHandle;
    }
    return texHandle;
}

TextureHandle Resources::GetTexture(std::string token)
{
    return tokenToTexH[token];
}

Vector2 Resources::GetTextureWidthHeight(std::string token)
{
    unsigned int width;
    unsigned int height;
    TextureResourcer::Instance().GetTextureSize(tokenToTexH[token], &width, &height);
    return Vector2( (float) width, (float)height);
}


FileDataHandle Resources::LoadDataFile(std::string token, std::string filename)
{
    FileDataHandle h = {-1};
    if(!FileResourcer::Instance().LoadFile(filename, &h))
    {
        tokenToFileH[token] = h;
    }
    return h;
}

FileDataHandle Resources::LoadStringData(std::string token, char* data)
{
    FileDataHandle h = {-1};
    if(!FileResourcer::Instance().LoadStringData(data, &h))
    {
        tokenToFileH[token] = h;
    }
    return h;
}

FileDataHandle Resources::GetData(std::string token)
{
    return tokenToFileH[token];
}


VertexShaderHandle Resources::LoadVertexShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel)
{
    ShaderResourcer& SR = ShaderResourcer::Instance(); 

    int result = SR.GenerateVertexShaderFromFile(
        gd->getDevice(),
        gd->getContext(),
        FileName.c_str(),
        EntryPoint,
        ShaderModel);

    assert(!result);
    errorBox(result, L" Could not generate vertex shader ");
    
    VertexShaderHandle vsHandle = SR.getVertexShaderHandle(FileName);

    int bl = SR.debug_GetInputLayoutByteLength(vsHandle);
    if( bl != sizeof(Vertex) )
    {
        std::wstringstream ss;
        ss << L" Vertex Size Differs from inputLayout. InputLayout:  " << bl
            << L" ; Vertex: " << sizeof(Vertex) << std::endl;
        errorBox(-1, ss.str().c_str());
    }

    return vsHandle;
}

VertexShaderHandle Resources::LoadVertexShader(uint8_t* data)
{
    assert(false && "NOT IMPLEMENTED YET: Loading Vertex Shader from string");
    VertexShaderHandle h = {-1};
    return h;
}

PixelShaderHandle Resources::LoadPixelShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel)
{
    PixelShaderHandle psHandle;

    ShaderResourcer& SR = ShaderResourcer::Instance(); 
    int result = SR.GeneratePixelShaderFromFile(
        gd->getDevice(),
        gd->getContext(),
        FileName.c_str(),
        EntryPoint,
        ShaderModel
        );

    errorBox(result, L" Could not generate pixel shader");

    psHandle = SR.getPixelShaderHandle(FileName);

    return psHandle;
}

PixelShaderHandle Resources::LoadPixelShader(uint8_t* data)
{
    assert(false && "NOT IMPLEMENTED YET: Loading Pixel Shader from string");
    PixelShaderHandle h = {-1};;
    return h;
}
