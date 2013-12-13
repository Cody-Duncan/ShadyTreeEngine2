#pragma once
#include "ISystem.h"
#include "Level.h"

class GameObject;
class PhysicsSystem;

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

    GameObject* playerObj;
    Level level;

    void SetPhysics(PhysicsSystem* _ps);
    PhysicsSystem* ps;
};

