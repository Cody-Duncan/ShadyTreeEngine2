#pragma once



#include <unordered_map>
#include <d3d11_1.h>
#include "DDSTextureLoader.h"
#include <string>
#include "Resources\GraphicsResourceHandles.h"

struct TextureData
{
    int ID;
    std::string name;
    int width;
    int height;
    ID3D11Resource* textureResource;
    ID3D11ShaderResourceView* textureView;
};

class TextureResourcer
{
public:
    std::vector<TextureData> textures;
    std::unordered_map<std::string, int> textureNameToId;

    int createTextureFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const uint8_t * binaryData, int binaryLength, TextureHandle* texHandle);
    int createTextureFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename, TextureHandle* texHandle);
    TextureData& getTextureData(TextureHandle tex);

    void Dispose();

    void GetTextureSize(TextureHandle tex, unsigned int* width, unsigned int* height);

    static TextureResourcer& Instance();

private: 

    static int IDGen_texture;
    int generateID();

    //Singleton
    TextureResourcer(void);
    ~TextureResourcer(void);
    TextureResourcer(TextureResourcer const&);              
    void operator=(TextureResourcer const&); 

    
};



