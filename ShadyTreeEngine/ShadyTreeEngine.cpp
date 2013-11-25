#include "ShadyTreeEngine.h"

#include "GraphicsFactory.h"
#include "OSHighResTimer.h"
#include "ComponentFactory.h"
#include "InputState.h"
#include "Stopwatch.h"

ShadyTreeEngine* CORE;

ShadyTreeEngine::ShadyTreeEngine()
{
    CORE = this;
}

ShadyTreeEngine::~ShadyTreeEngine()
{

}

void ShadyTreeEngine::Initialize()
{
    DebugLogOpen();

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
        deltaTime = timer.GetDeltaTime();
        timer.Update();

        Update(deltaTime);
        gINPUTSTATE->shift();
        Sleep(1);
    }
}

void ShadyTreeEngine::Free()
{
    for(unsigned int i = 0; i < systems.size(); ++i)
    {
        systems[i]->Free();
        delete systems[i];
    }

    ComponentFactory::Instance().clearAllCaches();

    DebugLogClose();
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