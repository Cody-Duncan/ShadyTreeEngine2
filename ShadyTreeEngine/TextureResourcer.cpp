#include "TextureResourcer.h"
#include "StringUtility.h"
#include "WICTextureLoader.h"

int TextureResourcer::IDGen_texture = 0;

TextureResourcer::TextureResourcer(void)
{
}


TextureResourcer::~TextureResourcer(void)
{
    for(unsigned int i = 0; i < textureViews.size(); i++)
    {
        textureViews[i]->Release();
    }

    textureViews.clear();
}

int TextureResourcer::createTextureFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename, TextureHandle* texHandle)
{
    ID3D11Resource* texture = nullptr;
    ID3D11ShaderResourceView* textureView = nullptr;
    WCHAR* wfilename = convertMultiByteToWCHAR(filename);
    HRESULT hr = CreateWICTextureFromFile(device, deviceContext, wfilename, &texture, &textureView);
    delete[] wfilename;
    //HRESULT hr = DirectX::CreateDDSTextureFromFile( device, , nullptr, &textureView );

    if(hr)
        return hr;

    int index = generateID();

    if(textureResources.size() <= (unsigned int)index)
        textureResources.resize(index+1);

    if(textureViews.size() <= (unsigned int)index)
        textureViews.resize(index+1);

    textureResources[index] = texture;
    textureViews[index] = textureView;
    textureNameToId[filename] = index;

    TextureHandle hTex = {index};
    *texHandle = hTex;
    return hr;
}

ID3D11ShaderResourceView* TextureResourcer::getTextureView(TextureHandle tex)
{
    return textureViews[tex.textureIndex];
}

int TextureResourcer::generateID()
{
    return IDGen_texture++;
}

void TextureResourcer::Dispose()
{
    for(auto iter = textureViews.begin(); iter != textureViews.end(); iter++)
    {
        (*iter)->Release();
    }

    for( auto iter = textureResources.begin(); iter != textureResources.end(); iter++)
    {
        (*iter)->Release();
    }

    textureViews.clear();
    textureNameToId.clear();
}