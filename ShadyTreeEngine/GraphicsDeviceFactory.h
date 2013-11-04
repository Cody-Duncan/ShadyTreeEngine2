#pragma once
#include "ShadyTree_DLLAPI.h"
#include "GraphicsDevice.h"
#include "DirectX_GraphicsDevice.h"

enum DeviceAPI
{
    DirectX11,
    OpenGL
};

ST_API GraphicsDevice* generateGraphicsDevice(DeviceAPI type);