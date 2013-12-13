#include "GameLogic.h"
#include "InputState.h"
#include "PhysicsSystem.h"

#include "GraphicsComponent.h"
#include "PositionalComponent.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"

#include "DeSerializer.h"
#include "Messenger.h"
#include "ContactMessage.h"

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

    
    playerObj->registerCollideHandler<GameLogic, &GameLogic::CollideEvent>(this);

    ps->setGravity(7000.0f);
}

inline void clampVel(float& v, float clamp)
{
    if( v < -clamp)
        v = -clamp;
    if( v > clamp)
        v = clamp;
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
                state.airborne = true;
            }
        }
        clampVel(phys.velocity.x, state.maxVelX);
        clampVel(phys.velocity.y, state.maxVelY);
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
        PhysicsComponent& phys = *playerObj->getComponent<PhysicsComponent>();
     
    }
    
}

void GameLogic::SetPhysics(PhysicsSystem* _ps)
{
    ps = _ps;
}
