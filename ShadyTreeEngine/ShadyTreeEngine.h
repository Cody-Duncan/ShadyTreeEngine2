#pragma once
#include "GraphicsSystem.h"

class ShadyTreeEngine
{
public:
    ST_API ShadyTreeEngine();
    ST_API ~ShadyTreeEngine();
    ST_API void Initialize(HINSTANCE hInstance, int show);
    ST_API void Run();
    ST_API void Free();

private: 
    void Update();
    void Draw();

    ISystem* graphicsSys;
};