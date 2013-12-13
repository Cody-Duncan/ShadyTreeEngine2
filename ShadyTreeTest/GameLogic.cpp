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
#include "Timer.h"

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

    ps->setGravity(6000.0f);
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
        if(state.airborne)
        {
            state.jumpTimer.Tick(deltaTime);

            if(gINPUTSTATE->keyDown('A'))
            {
                phys.velocity.x -= state.airborneAccel;
            }
            if(gINPUTSTATE->keyDown('D'))
            {
                phys.velocity.x += state.airborneAccel;
            }
            if(gINPUTSTATE->mouseDown(MouseButton::Left))
            {
                
            }
            if(gINPUTSTATE->mouseDown(MouseButton::Right))
            {
                
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

            if(gINPUTSTATE->keyDown('A'))
            {
                pos.position.x -= state.movementSpeed;
            }
            if(gINPUTSTATE->keyDown('D'))
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
            if(gINPUTSTATE->keyDown(VK_SPACE))
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
