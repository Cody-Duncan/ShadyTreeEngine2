#pragma once
#include "IGameState.h"
#include "Level.h"
#include "AttackComponent.h"

class GameObject;
class PhysicsSystem;

class GameLogic : public IGameState
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
    
    void SetScreenSize(int width, int height);

private:
    GameObject* playerObj;
    std::list<GameObject*> enemies;
    std::list<GameObject*> attacks;
    Level level;
    
    int Height;
    int Width;

    void ChaseAI(float deltaTime, int id);
    void PunchAI(float deltaTime, int id);
    void FlyAI(float deltaTime, int id);
    void LaserAI(float deltaTime, int id);
    void ExplodingAI(float deltaTime, int id);
    std::unordered_map<std::string, void (GameLogic::*)(float, int)> aiMap;

    void CollideEvent(Message* msg);
    void PlayerAttackCollision(Message* msg);
    void EnemyAttackCollision(Message* msg);

    void generateAttack(int owner, AttackDir, AttackType);

    Timer winTimer;
    bool winState;

    GameObject* winImage;
};

