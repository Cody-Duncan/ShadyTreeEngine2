#pragma once

#include "GraphicsDevice.h"
#include "DirectX_GraphicsDevice.h"
#include "IResources.h"
#include <unordered_map>

class Resources : public IResources
{
public:
    
    void setGraphicsDevice(GraphicsDevice* gd);
    void parseResourceIDs(std::string directory);
    virtual std::string getFileSourceOfRes(std::string resID);

    //Meshes
    Mesh* generateMesh(std::string identifierName);
    MeshHandle Resources::LoadMeshFile(std::string filename);
    MeshHandle Resources::BindMesh(Mesh* mesh);

    //Textures
    TextureHandle Resources::LoadTextureFile( std::string token, std::string filename);
    TextureHandle Resources::LoadTexture(std::string token, uint8_t* data, int length);
    TextureHandle Resources::LoadTextureRes(std::string id);
    TextureHandle Resources::GetTexture(std::string token);
    Vector2 Resources::GetTextureWidthHeight(std::string token);

    //GameData
    FileDataHandle Resources::LoadDataFile(std::string token, std::string filename);
    FileDataHandle Resources::LoadStringData(std::string token, char* data);
    FileDataHandle Resources::LoadDataRes(std::string id);
    FileDataHandle Resources::GetFileHandle(std::string token);
    std::iostream* Resources::GetFileData(FileDataHandle h);
    void Resources::CloseFile(FileDataHandle h);

    //Shaders
    VertexShaderHandle Resources::LoadVertexShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel);
    PixelShaderHandle Resources::LoadPixelShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel);

    VertexShaderHandle LoadVertexShaderRes(std::string ResID, const char * EntryPoint, const char * ShaderModel);
    PixelShaderHandle  LoadPixelShaderRes (std::string ResID, const char * EntryPoint, const char * ShaderModel);

    ST_API static IResources& Instance();

    void Free();

private:
    //necessary for instantiating graphics resources
    DirectX_GraphicsDevice* gd;

    Resources(void);
    ~Resources(void);

    bool hasResID(std::string resID);

    std::unordered_map<std::string, TextureHandle> tokenToTexH;
    std::unordered_map<std::string, FileDataHandle> tokenToFileH;

    std::unordered_map<std::string, std::string> resID_to_filename;

};

