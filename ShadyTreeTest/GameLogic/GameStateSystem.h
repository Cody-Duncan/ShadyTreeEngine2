#pragma once
#include "Core\ISystem.h"
#include "GameLogic.h"

class GameStateSystem : public ISystem
{
public:
    GameStateSystem(void);
    ~GameStateSystem(void);

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();
    virtual void Free();
    virtual void RecieveMessage(Message* msg);

    void SetScreenSize(int width, int height);

private:
    State prevState;
    State currentState;
    IGameState * game;
    IGameState* introScreen;
    IGameState* currentView;

    int screenWidth;
    int screenHeight;

};

