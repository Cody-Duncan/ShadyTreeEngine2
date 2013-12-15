#pragma once

#include "IGameState.h"

class IntroScreen : public IGameState
{
public:
    IntroScreen(void);
    ~IntroScreen(void);

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();
    virtual void Free();
    virtual void RecieveMessage(Message* msg);
    virtual void SetScreenSize(int width, int height);
private:
    enum SelectionState
    {
        NewGame = 0,
        EndGame,
        SelectionState_Num = EndGame + 1
    };

    int screenWidth;
    int screenHeight;

    GameObject* introScreenObj;
    GameObject* selector;

    SelectionState currentSelection;

    std::vector<Vector2> menuSelectionPositions;
};

