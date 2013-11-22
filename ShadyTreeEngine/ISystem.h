#pragma once

class ISystem
{
public:
    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Update() = 0;
    virtual void Unload() = 0;
};