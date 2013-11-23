#pragma once
#include "GraphicsSystem.h"
#include <vector>
#include "GameTimer.h"
#include "Message.h"

class ShadyTreeEngine
{
public:
    ST_API ShadyTreeEngine();
    ST_API ~ShadyTreeEngine();
    ST_API void Initialize();
    ST_API void Load();
    ST_API void AttachSystem(ISystem* system);
    ST_API void Run();
    ST_API void Free();

    ST_API void BroadcastMessage(Message* msg);

private: 
    void Update(float deltaTime);

    bool Running;
    std::vector<ISystem*> systems;
    GameTimer timer;

};

//A global pointer to the instance of the core
ST_API extern ShadyTreeEngine* CORE;