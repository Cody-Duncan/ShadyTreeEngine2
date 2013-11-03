#pragma once

#include <DirectXMath.h>

class Matrix4 : public DirectX::XMFLOAT4X4
{
public:
    Matrix4(void);
    ~Matrix4(void);


    inline void Identity()
    {
        _11 = 1;
        _12 = 0;
        _13 = 0;
        _14 = 0;

        _21 = 0;
        _22 = 1;
        _23 = 0;
        _24 = 0;

        _31 = 0;
        _32 = 0;
        _33 = 1;
        _34 = 0;

        _41 = 0;
        _42 = 0;
        _43 = 0;
        _44 = 1;
    }
};

