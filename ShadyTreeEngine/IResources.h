
#include "GraphicsDevice.h"

class ST_API IResources
{
public:
    virtual void setGraphicsDevice(GraphicsDevice* gd) = 0;

    //Meshes
    virtual Mesh* generateMesh(std::string identifierName) = 0;
    virtual MeshHandle LoadMeshFile(std::string filename) = 0;
    virtual MeshHandle BindMesh(Mesh* mesh) = 0;

    //Textures
    virtual TextureHandle LoadTextureFile(std::string filename) = 0;
    virtual TextureHandle LoadTexture(uint8_t* data, int length) = 0;

    //GameData
    virtual GameDataHandle LoadDataFile(std::string filename) = 0;
    virtual GameDataHandle LoadData(uint8_t data) = 0;

    //Shaders
    virtual VertexShaderHandle LoadVertexShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel) = 0;
    virtual VertexShaderHandle LoadVertexShader(uint8_t* data) = 0;

    virtual PixelShaderHandle LoadPixelShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel) = 0;
    virtual PixelShaderHandle LoadPixelShader(uint8_t* data) = 0;
};