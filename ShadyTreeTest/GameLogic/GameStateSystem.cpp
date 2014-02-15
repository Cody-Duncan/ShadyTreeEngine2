#include "StdAfx2.h"
#include "GameStateSystem.h"
#include "IntroScreen.h"
#include "Messaging\ChangeStateMessage.h"


GameStateSystem::GameStateSystem(void) : 
    prevState(NoState),
    currentState(IntroState),
    currentView(nullptr),
    game(nullptr),
    introScreen(nullptr)
{
}


GameStateSystem::~GameStateSystem(void) 
{
}

void GameStateSystem::Init()
{
    GameLogic* gameLogicInstance = new GameLogic();
    gameLogicInstance->SetScreenSize(screenWidth, screenHeight);
    game = gameLogicInstance;

    IntroScreen* introScreenInstance = new IntroScreen();
    introScreenInstance->SetScreenSize(screenWidth, screenHeight);
    introScreen = introScreenInstance;

    introScreen->Init();
    game->Init();

    switch(currentState)
    {
    case State::IntroState:
        currentView = introScreen;
        break;
    case State::GameState:
        currentView = game;
        break;
    }
}

void GameStateSystem::Load()
{
    currentView->Load();
}

void GameStateSystem::Update(float deltaTime)
{
    if(currentState != prevState && prevState != State::NoState)
    {
        currentView->Unload();

        switch(currentState)
        {
        case State::IntroState:
            currentView = introScreen;
            break;
        case State::GameState:
            currentView = game;
            break;
        }

        currentView->Load();
        prevState = currentState;
    }
    else
    {
        currentView->Update(deltaTime);
    }
}

void GameStateSystem::Unload()
{
    currentView->Unload();
}

void GameStateSystem::Free()
{
    introScreen->Free();
    game->Free();
}

void GameStateSystem::RecieveMessage(Message* msg)
{
    if(msg->type == MessageType::ChangeState)
    {
        prevState = currentState;
        currentState = static_cast<ChangeStateMessage*>(msg)->newState;
    }
    else
    {
        currentView->RecieveMessage(msg);
    }
}

void GameStateSystem::SetScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}