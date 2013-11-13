#pragma once

#include "GraphicsDevice.h"
#include "DirectX_GraphicsDevice.h"

class Resources
{
public:
    
    
void setGraphicsDevice(GraphicsDevice* gd);

//Meshes
Mesh* generateMesh();
MeshHandle Resources::LoadMeshFile(std::string filename);
MeshHandle Resources::BindMesh(Mesh* mesh);

//Textures
TextureHandle Resources::LoadTextureFile(std::string filename);
TextureHandle Resources::LoadTexture(uint8_t* data, int length);

//GameData
GameDataHandle Resources::LoadDataFile(std::string filename);
GameDataHandle Resources::LoadData(uint8_t data);

//Shaders
VertexShaderHandle Resources::LoadVertexShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel);
VertexShaderHandle Resources::LoadVertexShader(uint8_t* data);

PixelShaderHandle Resources::LoadPixelShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel);
PixelShaderHandle Resources::LoadPixelShader(uint8_t* data);

private:
    //necessary for instantiating graphics resources
    DirectX_GraphicsDevice* gd;

    Resources(void);
    ~Resources(void);

};

