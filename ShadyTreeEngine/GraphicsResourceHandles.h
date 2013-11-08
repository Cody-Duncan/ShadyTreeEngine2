#pragma once
#include "ShadyTree_DLLAPI.h"



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

struct BufferHandle
{
    int bufferID;
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
            using std::hash;
            return (hash<int>()(k.textureIndex));
        }
    };

}