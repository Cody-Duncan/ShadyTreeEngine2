#include "stdafx.h"
#include "Resources.h"

#include "Mesh.h"
#include "MeshResourcer.h"
#include "BufferResourcer.h"

Resources::Resources(void)
{
}


Resources::~Resources(void)
{
}

void Resources::setGraphicsDevice(GraphicsDevice* _gd)
{
    gd = dynamic_cast<DirectX_GraphicsDevice*>(gd);
}

Mesh* generateMesh(std::string& identifierName)
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


TextureHandle Resources::LoadTextureFile(std::string filename)
{
    TextureHandle texHandle;
    const char* filenameCSTR = filename.c_str();
    int result = TextureResourcer::Instance().createTextureFromWIC(gd->getDevice(), gd->getContext(), filenameCSTR, &texHandle);
    delete[] filenameCSTR;
    return texHandle;
}

TextureHandle Resources::LoadTexture(uint8_t* data, int length)
{
    TextureHandle texHandle;
    int result = TextureResourcer::Instance().createTextureFromWIC(gd->getDevice(), gd->getContext(), data, length, &texHandle);
    return texHandle;
}


GameDataHandle Resources::LoadDataFile(std::string filename)
{
    GameDataHandle h = {-1};
    return h;
}

GameDataHandle Resources::LoadData(uint8_t data)
{
    GameDataHandle h = {-1};
    return h;
}


VertexShaderHandle Resources::LoadVertexShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel)
{
     VertexShaderHandle h;
     gd->createVertexShader(FileName, EntryPoint, ShaderModel, &h);
     return h;
}

VertexShaderHandle Resources::LoadVertexShader(uint8_t* data)
{
    assert(false && "NOT IMPLEMENTED YET: Loading Vertex Shader from string");
    VertexShaderHandle h = {-1};
    return h;
}

PixelShaderHandle Resources::LoadPixelShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel)
{
    PixelShaderHandle h;
    gd->createPixelShader(FileName, EntryPoint, ShaderModel, &h);
    return h;
}

PixelShaderHandle Resources::LoadPixelShader(uint8_t* data)
{
    assert(false && "NOT IMPLEMENTED YET: Loading Pixel Shader from string");
    PixelShaderHandle h = {-1};;
    return h;
}
