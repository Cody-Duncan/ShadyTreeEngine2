#pragma once
#include "GraphicsSystem.h"
#include <vector>
#include "GameTimer.h"

class ShadyTreeEngine
{
public:
    ST_API ShadyTreeEngine();
    ST_API ~ShadyTreeEngine();
    ST_API void Initialize();
    ST_API void AttachSystem(ISystem* system);
    ST_API void Run();
    ST_API void Free();

private: 
    void Update(float deltaTime);

    bool Running;
    std::vector<ISystem*> systems;
    GameTimer timer;

};