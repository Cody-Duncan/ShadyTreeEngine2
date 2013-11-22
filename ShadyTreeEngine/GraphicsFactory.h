#pragma once
#include "GraphicsDevice.h"
#include "DirectX_GraphicsDevice.h"
#include "SpriteBatch.h"

ST_API GraphicsDevice* generateGraphicsDevice(DeviceAPI type);

ST_API SpriteBatch* generateSpriteBatch(GraphicsDevice* graphicsDevice);