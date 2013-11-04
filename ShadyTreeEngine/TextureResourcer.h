#pragma once



#include <unordered_map>
#include <d3d11_1.h>
#include "DDSTextureLoader.h"
#include <string>
#include "GraphicsResourceHandles.h"



class TextureResourcer
{
public:
    std::vector<ID3D11Resource*> textureResources;
    std::vector<ID3D11ShaderResourceView*> textureViews;
    std::unordered_map<std::string, int> textureNameToId;

    int createTextureFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename, TextureHandle* texHandle);
    ID3D11ShaderResourceView* getTextureView(TextureHandle tex);

    void Dispose();

    static TextureResourcer& Instance()
    {
        static TextureResourcer texRes;
        return texRes;
    }

private: 

    static int IDGen_texture;
    int generateID();

    //Singleton
    TextureResourcer(void);
    ~TextureResourcer(void);
    TextureResourcer(TextureResourcer const&);              
    void operator=(TextureResourcer const&); 

    
};



