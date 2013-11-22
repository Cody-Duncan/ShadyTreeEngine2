#pragma once

struct GameDataHandle
{
    int gameDataID;
};

struct TextureHandle
{
    int textureIndex;

    bool operator==(const TextureHandle &other) const
    { return textureIndex == other.textureIndex; }
};

struct MeshHandle
{
    int meshID;
};

struct IndexBufferHandle
{
    int IbufferID;
};

struct VertexBufferHandle
{
    int VbufferID;
};

struct VertexShaderHandle
{
    int vertexShaderID;
};

struct PixelShaderHandle
{
    int  pixelShadeeID;
};

namespace std {

    template <>
    struct hash<TextureHandle>
    {
        std::size_t operator()(const TextureHandle& k) const
        {
            return k.textureIndex;
        }
    };

}