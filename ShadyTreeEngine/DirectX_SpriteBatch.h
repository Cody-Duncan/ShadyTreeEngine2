#pragma once

#include "SpriteBatch.h"
#include "GraphicsDevice.h"


class DirectX_SpriteBatch : SpriteBatch
{
public:
    DirectX_SpriteBatch(void);
    ~DirectX_SpriteBatch(void);

    void Init();

    void Begin();
    void Draw(TextureHandle texH, Matrix transform);
    void Draw(TextureHandle texH, Matrix transform);
    void End();

};

