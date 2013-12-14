#include "GameLogic.h"
#include "InputState.h"
#include "PhysicsSystem.h"

#include "GraphicsComponent.h"
#include "PositionalComponent.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"
#include "AIComponent.h"

#include "DeSerializer.h"
#include "Messenger.h"
#include "ContactMessage.h"
#include "Timer.h"

GameLogic::GameLogic(void)
{
}


GameLogic::~GameLogic(void)
{
}

void GameLogic::Init()
{
    DebugAssert(Height > 0, "GAME LOGIC: World Height is 0.");
    DebugAssert(Width > 0, "GAME LOGIC: World Height is 0.");

    aiMap["ChaseAI"] = &GameLogic::ChaseAI;
    aiMap["PunchAI"] = &GameLogic::PunchAI;
    aiMap["FlyAI"] = &GameLogic::FlyAI;
    aiMap["LaserAI"] = &GameLogic::LaserAI;
    aiMap["ExplodingAI"] = &GameLogic::ExplodingAI;
}

void GameLogic::Load()
{
    DebugPrintf("GAME: Loading Game Logic\n");

    GameObjectCache::Instance().Reserve(400);
    GameObjectFactory& GOF = GameObjectFactory::Instance();
    IResources& res = Resources::Instance();

    res.parseResourceIDs("resources");

    //Build Level
    DeSerializer s;
    level.Initialize("ArenaLevel", s, Height, Width, 1.5f);

    //build player
    s.BuildArchetypes("Archetypes");
    s.BuildSubtypes("subtypes");

    playerObj = GOF.cloneArchetype("Player");
    PlayerStateComponent* state = ComponentFactory::Instance().createComponent<PlayerStateComponent>();
    state->airborne = false;
    playerObj->attachComponent(state);

    PositionalComponent& pos = *playerObj->getComponent<PositionalComponent>();
    pos.position = level.playerStart;
    
    playerObj->registerCollideHandler<GameLogic, &GameLogic::CollideEvent>(this);

    //build an enemy
    for(unsigned int i = 0; i < GOF.enemyTypes.size(); ++i)
    {
        GameObject* enemy = GOF.cloneArchetype(GOF.enemyTypes[i]);
        enemy->getComponent<PositionalComponent>()->position = level.enemyStartPositions[i];
        enemies.push_back(enemy);
    }
    

    //set environment parameters
    ps->setGravity(3000.0f);
}

float scale = 0;
void GameLogic::Update(float deltaTime)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();

    if(!CF.hasComponentCache<PositionalComponent>())
        return;

    PositionalComponent& pos = *playerObj->getComponent<PositionalComponent>();
    PhysicsComponent& phys = *playerObj->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *playerObj->getComponent<PlayerStateComponent>();

    if(level.IsOutsideLevel(pos.position))
    {
        level.IsOutsideLevel(pos.position);
        pos.position = level.playerStart;
        phys.velocity = Vector2(0,0);
    }

    if(!state.knocked)
    {
        if(state.airborne)
        {
            state.jumpTimer.Tick(deltaTime);

            if(gINPUTSTATE->keyDown(VK_LEFT))
            {
                phys.velocity.x -= state.airborneAccel;
            }
            if(gINPUTSTATE->keyDown(VK_RIGHT))
            {
                phys.velocity.x += state.airborneAccel;
            }
            if(gINPUTSTATE->keyPressed(VK_SPACE) && state.jumpCount < 2 && state.jumpTimer.IsDone())
            {
                phys.velocity.y = -state.jumpVelocity;
                ++state.jumpCount;
            }
        }
        else
        {
            phys.velocity.x = 0;
            float lastXPos = pos.position.x;

            if(gINPUTSTATE->keyDown(VK_LEFT))
            {
                pos.position.x -= state.movementSpeed;
            }
            if(gINPUTSTATE->keyDown(VK_RIGHT))
            {
                pos.position.x += state.movementSpeed;
            }
            if(!level.IsOnPlatform(pos.position, phys.body))
            {
                state.airborne = true;
                phys.velocity.x = (pos.position.x - lastXPos)/deltaTime;
                state.jumpTimer.Start(0.1f);
                ++state.jumpCount;
            }
            if(gINPUTSTATE->keyPressed(VK_SPACE))
            {
                phys.velocity.y -= state.jumpVelocity;
                phys.velocity.x = (pos.position.x - lastXPos)/deltaTime;
                state.airborne = true;
                state.jumpTimer.Start(0.1f);
                ++state.jumpCount;
            }
        }
        absClamp(phys.velocity.x, state.maxVelX);
        absClamp(phys.velocity.y, state.maxVelY);

        if(gINPUTSTATE->keyDown(VK_BACK))
        {
            state.knocked = true;
            state.knockedTimer.Start(1.0f);
            phys.velocity = Vector2(10000,-10000);
        }
    }
    else
    {
        if(state.knockedTimer.Tick(deltaTime))
        {
            state.knocked = false;
        }
    }

    for(int i = 0; i < enemies.size(); ++i)
    {
        if(enemies[i]->hasComponent<AIComponent>())
        {
            std::string& aiType = enemies[i]->getComponent<AIComponent>()->aiType;
            //(this->*aiMap[aiType])(deltaTime, enemies[i]->id);
        }
    }
}

void GameLogic::Unload()
{
    
}

void GameLogic::Free()
{

}

void GameLogic::RecieveMessage(Message* msg)
{

}


void GameLogic::CollideEvent(Message* msg)
{
    ContactMessage cmsg = *(static_cast<ContactMessage*> (msg));

    GameObject* collideThing = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[(cmsg.recieverIndex + 1) % 2]);

    if(collideThing->getComponent<PhysicsComponent>()->IsStatic &&      //hit a static object
        cmsg.contact.ContactNormal.Dot(Vector2(0,1)) > 0.8)                  //hit the top of it.
    {
        PlayerStateComponent& state = *playerObj->getComponent<PlayerStateComponent>();
        state.airborne = false;
        state.jumpCount = 0;
        state.jumpTimer.hardEnd();
        PhysicsComponent& phys = *playerObj->getComponent<PhysicsComponent>();
    }
}

void GameLogic::SetPhysics(PhysicsSystem* _ps)
{
    ps = _ps;
}

void GameLogic::SetWorldDimension(int height, int width)
{
    Height = height;
    Width = width;
}


void GameLogic::ChaseAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();

    phys.velocity.x = 0;
}
void GameLogic::PunchAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();

    phys.velocity.x = 0;
}
void GameLogic::FlyAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();

    phys.velocity.x = 0;
}
void GameLogic::LaserAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();

    phys.velocity.x = 0;
}
void GameLogic::ExplodingAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();

    phys.velocity.x = 0;
}