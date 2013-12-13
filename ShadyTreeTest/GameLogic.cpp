#include "GameLogic.h"
#include "InputState.h"
#include "PhysicsSystem.h"

#include "GraphicsComponent.h"
#include "PositionalComponent.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"

#include "DeSerializer.h"

#define MAX_VELOCITY 2000

GameLogic::GameLogic(void)
{
}


GameLogic::~GameLogic(void)
{
}

void GameLogic::Init()
{
    
}

void GameLogic::Load()
{
    DebugPrintf("GAME: Loading Game Logic\n");

    GameObjectCache::Instance().Reserve(400);
    GameObjectFactory& GOF = GameObjectFactory::Instance();
    IResources& res = Resources::Instance();

    res.parseResourceIDs("resources");

    DeSerializer s;
    s.BuildArchetypes("Archetypes");
    level.Initialize("ArenaLevel", s);

    playerObj = GOF.cloneArchetype("Player");
    PlayerStateComponent* state = ComponentFactory::Instance().createComponent<PlayerStateComponent>();
    state->airborne = false;
    playerObj->attachComponent(state);

    ps->setGravity(10000.0f);
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

    if(!state.knocked)
    {
        if(state.airborne && !state.knocked)
        {
            if(gINPUTSTATE->keyDown(VK_LEFT))
            {
                phys.velocity.x -= state.airborneAccel;
            }
            if(gINPUTSTATE->keyDown(VK_RIGHT))
            {
                phys.velocity.x += state.airborneAccel;
            }
        }
        else
        {
            phys.velocity.x = 0;

            if(gINPUTSTATE->keyDown(VK_LEFT))
            {
                pos.position.x -= state.movementSpeed;
            }
            if(gINPUTSTATE->keyDown(VK_RIGHT))
            {
                pos.position.x += state.movementSpeed;
            }
            if(gINPUTSTATE->keyDown(VK_SPACE))
            {
                phys.velocity.y -= state.jumpVelocity;
            }

            if(phys.velocity.LengthSquared() > MAX_VELOCITY * MAX_VELOCITY)
            {
                phys.velocity = phys.velocity.Normal() * MAX_VELOCITY;
            }
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

void GameLogic::SetPhysics(PhysicsSystem* _ps)
{
    ps = _ps;
}
