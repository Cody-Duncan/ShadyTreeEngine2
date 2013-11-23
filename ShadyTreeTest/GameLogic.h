#pragma once
#include "ISystem.h"

class GameObject;

class GameLogic : public ISystem
{
public:
    GameLogic(void);
    ~GameLogic(void);

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();
    virtual void Free();
    virtual void RecieveMessage(Message* msg);

    GameObject* oneobject;
};

