#pragma once

#include <DirectXMath.h>
//#include <new>

class Vector2 : public DirectX::XMFLOAT2
{
public:
    Vector2(void);
    Vector2(float x, float y) : XMFLOAT2(x, y) {}
    ~Vector2(void);
};

class Vector3 : public DirectX::XMFLOAT3
{
public:
    Vector3(void);
    Vector3(float x, float y, float z) : XMFLOAT3(x, y, z) {}
    ~Vector3(void);
};

class Vector4 : public DirectX::XMFLOAT4
{
public:
    Vector4(void);
    Vector4(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
    ~Vector4(void);
};



typedef Vector4 Color;