#include "ShadyTreeEngine.h"

#include "Diagnostic\OSHighResTimer.h"
#include "GameObject\ComponentFactory.h"
#include "Input\InputState.h"
#include "Diagnostic\Stopwatch.h"
#include "GameObject\GameObjectCache.h"
#include "Resources\Resources.h"
#include "GameObject\GameObjectCache.h"

ShadyTreeEngine* CORE;

ShadyTreeEngine::ShadyTreeEngine()
{
    DebugLogOpen();
    CORE = this;
}

ShadyTreeEngine::~ShadyTreeEngine()
{
    DebugLogClose();
}

void ShadyTreeEngine::Initialize()
{
    for(unsigned int i = 0; i < systems.size(); ++i)
    {
        systems[i]->Init();
    }

    timer.Start();
    Running = true;
}

void ShadyTreeEngine::Load()
{
    for(unsigned int i = 0; i < systems.size(); ++i)
    {
        systems[i]->Load();
    }
}

void ShadyTreeEngine::Run()
{
    float deltaTime = 0.0f;

    Stopwatch watch;
    while( Running )
    {
        timer.Update();
        deltaTime = timer.GetDeltaTime();
        if (deltaTime > 1.0f/60.0f) 
            deltaTime = 1.0f/60.0f;

        Update(deltaTime);

        GameObjectCache::Instance().DestroyCleanup();

        gINPUTSTATE->shift();
        //Sleep(1);
    }
}

void ShadyTreeEngine::Free()
{
    ComponentFactory::Instance().FreeAllCaches();
    GameObjectCache::Instance().Free();

    Resources::Instance().Free();

    //note, this goes backwards to free the systems in reverse order of thier initialization
    //also note, that unsigned int overflows when it goes below 0, hence the odd-looking condition.
    for(unsigned int i = systems.size()-1; i < systems.size(); --i)
    {
        systems[i]->Free();
        delete systems[i];
    }
}

void ShadyTreeEngine::Update(float deltaTime)
{
    for(unsigned int i = 0; i < systems.size(); ++i)
    {
        systems[i]->Update(deltaTime);
    }
}

void ShadyTreeEngine::AttachSystem(ISystem* system)
{
    systems.push_back(system);
}

void ShadyTreeEngine::BroadcastMessage(Message* msg)
{
    if (msg->type == MessageType::Quit)
        Running = false;

    for(unsigned int i = 0; i < systems.size(); ++i)
    {
        systems[i]->RecieveMessage(msg);
    }
}