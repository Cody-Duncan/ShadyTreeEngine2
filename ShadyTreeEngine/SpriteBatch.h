#pragma once
#pragma once

#include "DeviceAPI.h"

class SpriteBatch
{
public:
    SpriteBatch();
    SpriteBatch(DeviceAPI api) : API(api) {}
    ~SpriteBatch(void);

    DeviceAPI API;
};

