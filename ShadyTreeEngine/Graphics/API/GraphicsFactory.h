#pragma once
#include "Graphics\API\GraphicsDevice.h"
#include "Graphics\DirectX\DirectX_GraphicsDevice.h"
#include "Graphics\API\SpriteBatch.h"
#include "Graphics\API\PrimitiveBatch.h"

ST_API GraphicsDevice* generateGraphicsDevice(DeviceAPI type);

ST_API SpriteBatch* generateSpriteBatch(GraphicsDevice* graphicsDevice);

ST_API PrimitiveBatch* generatePrimitiveBatch(GraphicsDevice* graphicsDevice);