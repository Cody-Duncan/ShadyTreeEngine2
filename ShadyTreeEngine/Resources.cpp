#include "stdafx.h"
#include "Resources.h"

#include "Mesh.h"
#include "MeshResourcer.h"
#include "BufferResourcer.h"
#include "FileResourcer.h"

#include "TinyDir.h"
#include "StringUtility.h"

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



void getListOfFilesInDir(const char* directory, std::vector<std::string>& filenames)
{
    tinydir_dir dir;
    tinydir_open(&dir, directory);
    
    while (dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        filenames.push_back(file.name);

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}

void Resources::parseResourceIDs(std::string directory)
{
    std::vector<std::string> filenames;
    getListOfFilesInDir(directory.c_str(), filenames);

    int lastDot;
    std::string filePath;
    std::string fileNameSansExt;

    for(unsigned int i = 2; i < filenames.size(); ++i)
    {
        lastDot = filenames[i].find_last_of('.');
        filePath = directory + "/" + filenames[i];
        fileNameSansExt = filenames[i].substr(0,lastDot) ;

        if( hasResID(fileNameSansExt) )
            resID_to_filename[filenames[i]] = filePath; //set just the filename as key
        else
            resID_to_filename[fileNameSansExt] = filePath; //key already exists, use extension too
    }
}

bool Resources::hasResID(std::string resID)
{
    return resID_to_filename.find( resID ) != resID_to_filename.end();
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

TextureHandle Resources::LoadTextureRes(std::string resID)
{
    TextureHandle h = {-1};
    if(hasResID(resID))
    {
        h = Resources::LoadTextureFile(resID, resID_to_filename[resID]);
    }
    return h;
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

FileDataHandle Resources::LoadDataRes(std::string resID)
{
    FileDataHandle h = {-1};
    if(hasResID(resID))
    {
        h = LoadDataFile(resID, resID_to_filename[resID]);
    }
    return h;
}

FileDataHandle Resources::GetFileHandle(std::string token)
{
    return tokenToFileH[token];
}

std::iostream* Resources::GetFileData(FileDataHandle h)
{
    return FileResourcer::Instance().getFile(h);
}

void Resources::CloseFile(FileDataHandle h)
{
    FileResourcer::Instance().closeFile(h);
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

VertexShaderHandle Resources::LoadVertexShaderRes(std::string ResID, const char * EntryPoint, const char * ShaderModel)
{
    VertexShaderHandle h = {-1};
    if(hasResID(ResID))
    {
        h = LoadVertexShaderFile(resID_to_filename[ResID], EntryPoint, ShaderModel);
    }
    return h;
}
PixelShaderHandle  Resources::LoadPixelShaderRes (std::string ResID, const char * EntryPoint, const char * ShaderModel)
{
    PixelShaderHandle h = {-1};
    if(hasResID(ResID))
    {
        h = LoadPixelShaderFile(resID_to_filename[ResID], EntryPoint, ShaderModel);
    }
    return h;
}
