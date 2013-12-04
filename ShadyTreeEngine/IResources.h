
#include "GraphicsDevice.h"

class ST_API IResources
{
public:
    virtual void setGraphicsDevice(GraphicsDevice* gd) = 0;

    virtual void parseResourceIDs(std::string directory) = 0;
    virtual std::string getFileSourceOfRes(std::string resID) = 0;

    virtual void Free() = 0;

    //Meshes
    virtual Mesh* generateMesh(std::string identifierName) = 0;
    virtual MeshHandle LoadMeshFile(std::string filename) = 0;
    virtual MeshHandle BindMesh(Mesh* mesh) = 0;

    //Textures
    virtual TextureHandle LoadTextureFile(std::string token, std::string filename) = 0;
    virtual TextureHandle LoadTexture(std::string token, uint8_t* data, int length) = 0;
    virtual TextureHandle LoadTextureRes(std::string resID) = 0;
    virtual TextureHandle GetTexture(std::string token) = 0;
    virtual Vector2 GetTextureWidthHeight(std::string token) = 0;

    //GameData
    virtual FileDataHandle LoadDataFile(std::string token, std::string filename) = 0;
    virtual FileDataHandle LoadDataRes(std::string resID) = 0;
    virtual FileDataHandle LoadStringData(std::string token, char* data) = 0;
    virtual FileDataHandle GetFileHandle(std::string token) = 0;
    virtual std::iostream* GetFileData(FileDataHandle h) = 0;
    virtual void CloseFile(FileDataHandle h) = 0;

    //Shaders
    virtual VertexShaderHandle LoadVertexShaderFile(std::string FileName, const char * EntryPoint, const char * ShaderModel) = 0;
    virtual PixelShaderHandle  LoadPixelShaderFile (std::string FileName, const char * EntryPoint, const char * ShaderModel) = 0;
    virtual bool VerifyVertexSize(std::string FileName, size_t vertexSize) = 0;

    virtual VertexShaderHandle LoadVertexShaderRes(std::string ResID, const char * EntryPoint, const char * ShaderModel) = 0;
    virtual PixelShaderHandle  LoadPixelShaderRes (std::string ResID, const char * EntryPoint, const char * ShaderModel) = 0;
};