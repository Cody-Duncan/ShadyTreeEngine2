#include "TextureResourcer.h"
#include "Utility\StringUtility.h"
#include "WICTextureLoader.h"

void GetTextureSizeD3D( ID3D11Resource* res, unsigned int* width, unsigned int* height )
{

    DebugAssert( res != 0, "Tried to get size of texture on a null ID3D11Resource (the texture resource bound to graphics card)");

    // This is the most generic solution. you can make it a lot
    // simpler if you know it will always be a 2D texture file
    D3D11_RESOURCE_DIMENSION dim;
    res->GetType( &dim );
    switch(dim)
    {
        case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
            { 
                 auto txt = reinterpret_cast<ID3D11Texture1D*>( res );
                 D3D11_TEXTURE1D_DESC desc;
                 txt->GetDesc( &desc );
                 if ( width ) *width = desc.Width;
                 if ( height ) *height = 1;
            }
            break;
        case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
            {
                 auto txt = reinterpret_cast<ID3D11Texture2D*>( res );
                 D3D11_TEXTURE2D_DESC desc;
                 txt->GetDesc( &desc );
                 if ( width ) *width = desc.Width;
                 if ( height ) *height = desc.Height;
            }
            break;
        case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
            {
                 auto txt = reinterpret_cast<ID3D11Texture3D*>( res );
                 D3D11_TEXTURE3D_DESC desc;
                 txt->GetDesc( &desc );
                 if ( width ) *width = desc.Width;
                 if ( height ) *height = desc.Height;
            }
            break;
        default:
             if ( width ) *width = 0;
             if ( height ) *height = 0;
            break;
    }
}

int TextureResourcer::IDGen_texture = 0;

TextureResourcer& TextureResourcer::Instance()
{
    static TextureResourcer texRes;
    return texRes;
}

TextureResourcer::TextureResourcer(void)
{
}


TextureResourcer::~TextureResourcer(void)
{
    Dispose();
}

int TextureResourcer::createTextureFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const uint8_t * binaryData, int binaryLength, TextureHandle* texHandle)
{
    ID3D11Resource* texture = nullptr;
    ID3D11ShaderResourceView* textureView = nullptr;
    HRESULT hr = CreateWICTextureFromMemory(device, deviceContext, binaryData ,binaryLength, &texture, &textureView);
    //HRESULT hr = DirectX::CreateDDSTextureFromFile( device, , nullptr, &textureView );

    

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    textureView->GetDesc(&desc);
    

    if(hr)
        return hr;

    //generate id (which is really an array index)
    int index = generateID();
    
    //get size
    unsigned int width;
    unsigned int height;
    GetTextureSizeD3D(texture, &width, &height);

    //ensure sizing in textures storage
    if(textures.size() <= (unsigned int)index)
        textures.resize(index+1);

    //add to storage
    TextureData& data = textures[index];
    data.ID = index;
    data.name = "";
    data.width = width;
    data.height = height;
    data.textureResource = texture;
    data.textureView = textureView;

    TextureHandle hTex = {index};
    *texHandle = hTex;

    return hr;
}

int TextureResourcer::createTextureFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename, TextureHandle* texHandle)
{
    ID3D11Resource* texture = nullptr;
    ID3D11ShaderResourceView* textureView = nullptr;
    WCHAR* wfilename = convertMultiByteToWCHAR(filename);
    HRESULT hr = CreateWICTextureFromFile(device, deviceContext, wfilename, &texture, &textureView);
    delete[] wfilename;
    //HRESULT hr = DirectX::CreateDDSTextureFromFile( device, , nullptr, &textureView );

    if(!textureView)
    {
        assert(false);
        DebugAssert(textureView, "Failed to load texture: ", filename);
    }
    

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    textureView->GetDesc(&desc);
    

    if(hr)
        return hr;

    //generate id (which is really an array index)
    int index = generateID();
    
    //get size
    unsigned int width;
    unsigned int height;
    GetTextureSizeD3D(texture, &width, &height);

    //ensure sizing in textures storage
    if(textures.size() <= (unsigned int)index)
        textures.resize(index+1);

    //add to storage
    TextureData& data = textures[index];
    data.ID = index;
    data.name = filename;
    data.width = width;
    data.height = height;
    data.textureResource = texture;
    data.textureView = textureView;

    //reference by name
    textureNameToId[filename] = index;

    TextureHandle hTex = {index};
    *texHandle = hTex;

    return hr;
}

TextureData& TextureResourcer::getTextureData(TextureHandle tex)
{
    return textures[tex.textureIndex];
}

int TextureResourcer::generateID()
{
    return IDGen_texture++;
}

void TextureResourcer::Dispose()
{
    for(auto iter = textures.begin(); iter != textures.end(); iter++)
    {
        (*iter).textureView->Release();
        (*iter).textureResource->Release();
    }

    textures.clear();
}




void TextureResourcer::GetTextureSize(TextureHandle tex, unsigned int* width, unsigned int* height)
{
    ::GetTextureSizeD3D( textures[tex.textureIndex].textureResource , width, height);
}
