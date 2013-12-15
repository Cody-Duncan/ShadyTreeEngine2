#include "GameLogic.h"
#include "InputState.h"
#include "PhysicsSystem.h"

#include "GraphicsComponent.h"
#include "PositionalComponent.h"
#include "PhysicsComponent.h"
#include "PlayerStateComponent.h"
#include "AIComponent.h"
#include "AttackComponent.h"
#include "GraphicsComponent.h"

#include "DeSerializer.h"
#include "Messenger.h"
#include "ContactMessage.h"
#include "Timer.h"
#include "ShadyTreeEngine.h"
#include "DebugDrawMessage.h"
#include "ChangeGravityMessage.h"
#include "ChangeStateMessage.h"


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
    
    PhysicsComponent& phys = *playerObj->getComponent<PhysicsComponent>();
    phys.registerCollideHandler<GameLogic, &GameLogic::CollideEvent>(this);

    
    //build an enemy
    for(unsigned int i = 0; i < GOF.enemyTypes.size(); ++i) 
    {
        GameObject* enemy = GOF.cloneArchetype(GOF.enemyTypes[i]);
        enemy->getComponent<PositionalComponent>()->position = level.enemyStartPositions[i];
        enemies.push_back(enemy);
        enemy->getComponent<PhysicsComponent>()->registerCollideHandler<GameLogic, &GameLogic::CollideEvent>(this);
    }
    
    //set environment parameters
    CORE->BroadcastMessage(&ChangeGravityMessage(1000.0f));
    CORE->BroadcastMessage(&DebugDrawAllMessage());

    Resources::Instance().LoadTextureRes("WinMessage");
    winImage = GOF.createGraphicalEntity("WinMessage");
    winImage->getComponent<GraphicsComponent>()->active = false;
    PositionalComponent& winPos = *winImage->getComponent<PositionalComponent>();
    winPos.position = level.LevelPos;
    GraphicsComponent& winG = *winImage->getComponent<GraphicsComponent>();
    winPos.position -= winG.textureArea.dimensions/2;

    Resources::Instance().LoadTextureRes("LoseMessage");
    loseImage = GOF.createGraphicalEntity("LoseMessage");
    loseImage->getComponent<GraphicsComponent>()->active = false;
    PositionalComponent& losePos = *loseImage->getComponent<PositionalComponent>();
    losePos.position = level.LevelPos;
    GraphicsComponent& loseG = *loseImage->getComponent<GraphicsComponent>();
    losePos.position -= loseG.textureArea.dimensions/2;

    winState = false;
    loseState = false;

    PlayerLives = 3;
}

////////////////////////////////
//UPDATE METHOD
//MOSTLY PLAYER LOGIC
////////////////////////////////

float scale = 0;
void GameLogic::Update(float deltaTime)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();

    //Check Win/Lose conditions
    if(enemies.size() == 0 && !winState && !loseState)
    {
        winState = true;
        winTimer.Start(3.0f);
        winImage->getComponent<GraphicsComponent>()->active = true;
    }
    if(PlayerLives <= 0 && !winState && !loseState)
    {
        loseState = true;
        winTimer.Start(3.0f);
        loseImage->getComponent<GraphicsComponent>()->active = true;

        //delete the enemies
        auto enemyI = enemies.begin();
        while( enemyI != enemies.end() )
        {
            GOC.DestroyNow((*enemyI)->id);
            enemies.erase(enemyI++);
        }
    }

    //Tick win/lose timer. When it finishes, go back to intro screen
    if(winState || loseState)
    {
        if(winTimer.Tick(deltaTime))
        {
            CORE->BroadcastMessage(&ChangeStateMessage(State::IntroState));
        }
    }

    //grab player components
    PositionalComponent& pos = *playerObj->getComponent<PositionalComponent>();
    PhysicsComponent& phys = *playerObj->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *playerObj->getComponent<PlayerStateComponent>();

    //reset the player if they are outside the level.
    if(level.IsOutsideLevel(pos.position))
    {
        level.IsOutsideLevel(pos.position);
        pos.position = level.playerStart;
        phys.velocity = Vector2(0,0);
        state.damage= 0;
        --PlayerLives;
    }

   
    state.attackWait.Tick(deltaTime);
    state.knockedTimer.Tick(deltaTime);
    state.jumpTimer.Tick(deltaTime);

    if(!state.knocked) //do nothing if knocked, just let em fly
    {
        //check for attack input
        if(gINPUTSTATE->keyPressed('Q'))
        {
            DebugPrintf("Hit Q:%f/%f\n", state.attackWait.passed, state.attackWait.wait);
            if(state.attackWait.IsDone()) //don't spam attacks
            {
                if( gINPUTSTATE->keyDown(VK_LEFT) ) //left punch
                {
                    generateAttack(playerObj->id, AttackDir::Attack_Left, AttackType::Regular);
                    state.attackWait.Start(0.1f); 
                }

                if(gINPUTSTATE->keyDown(VK_RIGHT)) //right punch
                {
                    generateAttack(playerObj->id, AttackDir::Attack_Right, AttackType::Regular);
                    state.attackWait.Start(0.1f);
                }

                if(gINPUTSTATE->keyDown(VK_UP)) //up punch
                {
                    generateAttack(playerObj->id, AttackDir::Attack_Up, AttackType::Regular);
                    state.attackWait.Start(0.1f);
                }

                if(gINPUTSTATE->keyDown(VK_DOWN)) //down double-sided "kick"
                {
                    generateAttack(playerObj->id, AttackDir::Attack_Down, AttackType::Regular);
                    state.attackWait.Start(0.1f);
                }
            }
        }

        //laser test
        if(gINPUTSTATE->keyPressed('E'))
        {
            generateAttack(playerObj->id, AttackDir::Attack_Right, AttackType::Laser);
        }

        if(state.airborne) //airborne
        {
            //input causes velocity change, not absolute.
            if(gINPUTSTATE->keyDown(VK_LEFT))
            {
                phys.velocity.x -= state.airborneAccel;
            }
            if(gINPUTSTATE->keyDown(VK_RIGHT))
            {
                phys.velocity.x += state.airborneAccel;
            }
            if(gINPUTSTATE->keyPressed(VK_SPACE) && state.jumpCount < 2 && state.jumpTimer.IsDone()) //jump
            {
                phys.velocity.y = -state.jumpVelocity;
                ++state.jumpCount;
            }

        }
        else // on ground
        {
            phys.velocity.x = 0;
            float lastXPos = pos.position.x;

            if(state.attackWait.IsDone())
            {
                if(gINPUTSTATE->keyDown(VK_LEFT))
                {
                    pos.position.x -= state.movementSpeed;
                }
                if(gINPUTSTATE->keyDown(VK_RIGHT))
                {
                    pos.position.x += state.movementSpeed;
                }
                if(!level.IsOnPlatform(pos.position, phys.body)) //switch to airborne if off platform
                {
                    state.airborne = true;
                    phys.velocity.x = (pos.position.x - lastXPos)/deltaTime;
                    state.jumpTimer.Start(0.1f);
                    ++state.jumpCount;
                }
                if(gINPUTSTATE->keyPressed(VK_SPACE)) //jump
                {
                    phys.velocity.y -= state.jumpVelocity;
                    phys.velocity.x = (pos.position.x - lastXPos)/deltaTime;
                    state.airborne = true;
                    state.jumpTimer.Start(0.1f);
                    ++state.jumpCount;
                }
            }
        }

        //clamp movement speed
        absClamp(phys.velocity.x, state.maxVelX);
        absClamp(phys.velocity.y, state.maxVelY);

        if(gINPUTSTATE->keyPressed(VK_BACK)) //insta-death button
        {
            state.knocked = true;
            state.knockedTimer.Start(1.0f);
            phys.velocity = Vector2(10000,-10000);
            CORE->BroadcastMessage(&ChangeStateMessage(State::IntroState));
        }
    }
    else //do nothing if knocked, player loses control momentarily
    {
        if(state.knockedTimer.IsDone())
        {
            state.knocked = false;
        }
    }

    //update enemy AI
    auto enemyI = enemies.begin();
    while( enemyI != enemies.end() )
    {
        if((*enemyI)->hasComponent<AIComponent>())
        {
            AIComponent* ai = (*enemyI)->getComponent<AIComponent>();
            std::string& aiType = ai->aiType;
            (this->*aiMap[aiType])(deltaTime, (*enemyI)->id);
        }

        //if enemy falls outside, delete it.
        if(level.IsOutsideLevel((*enemyI)->getComponent<PositionalComponent>()->position))
        {
            GOC.DestroyNow((*enemyI)->id);
            enemies.erase(enemyI++);
        }
        else
        {
            ++enemyI;
        }
    }

    //update attack objects
    auto iter = attacks.begin();
    while( iter != attacks.end() )
    {
        GameObject* go = *iter;

        AttackComponent* atk = go->getComponent<AttackComponent>();
        GameObject* attackOwner = GOC.Get(atk->ownerID);
        PositionalComponent* attackPos = go->getComponent<PositionalComponent>();
        PositionalComponent* ownerPos = attackOwner->getComponent<PositionalComponent>();
        
        //if not a laser, attack moves with the owner
        if(atk->type != AttackType::Laser)
            attackPos->position = ownerPos->position;

        //destroy attacks after a time to live.
        if(atk->attackTimer.Tick(deltaTime))
        {
            GOC.DestroyNow(atk->parent()->id);
            attacks.erase(iter++);
        }
        else
        {
            ++iter;
        }
       
    }

    
}

void GameLogic::Unload()
{
    playerObj = nullptr;
    enemies.clear();
    attacks.clear();
    level.Dispose();

    GameObjectFactory& GOF = GameObjectFactory::Instance();
    GOF.enemyTypes.clear();
    GOF.clearArchetypes();
    

    GameObjectCache::Instance().Clear();
    ComponentFactory::Instance().clearAllCaches();
}

void GameLogic::Free()
{

}

void GameLogic::RecieveMessage(Message* msg)
{

}

////////////////////////////////
//Collision Response
////////////////////////////////

//handles collision with ground.
void GameLogic::CollideEvent(Message* msg)
{
    ContactMessage cmsg = *(static_cast<ContactMessage*> (msg));
    
    GameObject* reciever = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[cmsg.recieverIndex]);
    GameObject* collideThing = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[(cmsg.recieverIndex + 1) % 2]);

    if(collideThing->getComponent<PhysicsComponent>()->IsStatic &&      //hit a static object
        cmsg.contact.ContactNormal.Dot(Vector2(0,1)) > 0.8)                  //hit the top of it.
    {
        if(reciever->hasComponent<PlayerStateComponent>() && reciever->hasComponent<PhysicsComponent>())
        {
            //put the object in the "on ground" state
            PlayerStateComponent& state = *reciever->getComponent<PlayerStateComponent>();
            state.airborne = false;
            state.jumpCount = 0;
            state.jumpTimer.hardEnd();
            PhysicsComponent& phys = *reciever->getComponent<PhysicsComponent>();
        }
    }
}

void hitObject(GameObject* go, GameObject* attack)
{
    PositionalComponent& pos    = *go->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *go->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *go->getComponent<PlayerStateComponent>();

    AttackComponent& atkInfo    = *attack->getComponent<AttackComponent>();
    PositionalComponent& atkPos    = *attack->getComponent<PositionalComponent>();

    //do damage, set state to "knocked"
    state.damage += atkInfo.damage;
    state.knocked = true;
    state.knockedTimer.Start(1.0f*state.damage/100.0f);
    
    //send em flyin'
    Vector2 hitDir = (pos.position - atkPos.position).Normal();
    hitDir.y -= 0.7f;
    phys.velocity += hitDir * (state.damage/100.0f) * 1500;
    state.airborne = true;

    
    
}

//handles collision with a player owned attack object.
void GameLogic::PlayerAttackCollision(Message* msg)
{
    ContactMessage cmsg = *(static_cast<ContactMessage*> (msg));

    GameObject* attack = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[cmsg.recieverIndex]);
    GameObject* collideThing = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[(cmsg.recieverIndex + 1) % 2]);

    if( ! collideThing->getComponent<PhysicsComponent>()->IsStatic &&  //hit a moving object
        collideThing->id != playerObj->id)                             //wasn't the player
    {
        hitObject(collideThing, attack);
        GameObjectCache::Instance().DestroyNow(attack->id);     //get rid of the attack object
        attacks.remove(attack);                                 //remove attack from attacks list.
    }
}
    
//handles collision with an enemy owned attack object.
void GameLogic::EnemyAttackCollision(Message* msg)
{
    ContactMessage cmsg = *(static_cast<ContactMessage*> (msg));

    GameObject* attack = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[cmsg.recieverIndex]);
    GameObject* collideThing = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[(cmsg.recieverIndex + 1) % 2]);

    if(collideThing->id == playerObj->id)      //hit the player
    {
        hitObject(collideThing, attack);
        GameObjectCache::Instance().DestroyNow(attack->id);     //get rid of the attack object
        attacks.remove(attack);                                 //remove attack from attacks list.
    }
}

void GameLogic::SetScreenSize(int width, int height)
{
    Height = height;
    Width = width;
}


////////////////////////////////
//   AI
////////////////////////////////

//Just follows the player
void GameLogic::ChaseAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();

    if(!state.knocked)
    {
        

        Vector2 playerPos = playerObj->getComponent<PositionalComponent>()->position;
        float playerDist =  playerPos.x - pos.position.x;

        if(state.airborne)
        {
            if(playerDist < 0)
            {
                phys.velocity.x -= state.airborneAccel;
            }
            else
            {
                phys.velocity.x += state.airborneAccel;
            }
            absClamp(phys.velocity.x, state.maxVelX);
        }
        else
        {
            phys.velocity.x = 0;
            if(playerDist < 0)
            {
                pos.position.x -= state.movementSpeed/2;
            }
            else
            {
                pos.position.x += state.movementSpeed/2;
            }
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

enum AIOuterState
{
    Move = 0,
    Punch,
    Wait
};
enum AIInnerState
{
    MoveLeft = 0,
    MoveRight,
};

//runs back and forth, punches the player if they get near.
void GameLogic::PunchAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();
    AIComponent& ai = *enemy->getComponent<AIComponent>();

    if(state.knocked)
    {
        if(state.knockedTimer.Tick(deltaTime))
        {
            state.knocked = false;
        }
        return;
    }

    ai.stateTimer.Tick(deltaTime);

    Vector2 playerPos = playerObj->getComponent<PositionalComponent>()->position;
    float playerDist =  playerPos.x - pos.position.x;
    if(abs(playerDist) < 70)
        ai.outerState = Punch;

    if(!state.airborne)
    {
        phys.velocity.x = 0;
        Vector2 newPosition;
        switch(ai.outerState)
        {
        case Move:
            switch(ai.innerState)
            {
            case MoveLeft:
                newPosition = pos.position;
                newPosition.x -= state.movementSpeed/2;
                if(level.IsOnPlatform(newPosition, phys.body))
                {
                    pos.position = newPosition;
                }

                if(ai.stateTimer.IsDone())
                {
                    ai.outerState = Punch;
                }
                break;

            case MoveRight:
                newPosition = pos.position;
                newPosition.x += state.movementSpeed/2;
                if(level.IsOnPlatform(newPosition, phys.body))
                {
                    pos.position = newPosition;
                }

                if(ai.stateTimer.IsDone())
                {
                    ai.outerState = Punch;
                }
                break;
            }
            break;
        case Punch:
            if(ai.attackTimer.Tick(deltaTime))
            {
                switch(ai.innerState)
                {
                    case MoveLeft:
                        generateAttack(enemy->id,AttackDir::Attack_Left, AttackType::Regular);
                        break;
                    case MoveRight:
                        generateAttack(enemy->id,AttackDir::Attack_Right, AttackType::Regular);
                        break;
                }
                ai.attackTimer.Start(0.2f);
            }
            
            ai.innerState = ai.innerState == MoveLeft ? MoveRight : MoveLeft;
            ai.outerState = Move;
            ai.stateTimer.Start(4.0f);
            
            break;
        
        }
    }
    else
    {
        float centerDir = level.LevelPos.x - pos.position.x;
        if(centerDir < 0)
        {
            phys.velocity.x -= state.airborneAccel/2;
        }
        else
        {
            phys.velocity.x += state.airborneAccel/2;
        }
    }
}

//jumops at the player and does a spin attack!
void GameLogic::FlyAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();
    AIComponent& ai = *enemy->getComponent<AIComponent>();

    if(!state.knocked)
    {
        
        Vector2 playerPos = playerObj->getComponent<PositionalComponent>()->position;
        float playerDist =  playerPos.x - pos.position.x;

        if(!state.airborne)
            phys.velocity.x = 0;
        else
        {
            if(playerDist < -100)
            {
                phys.velocity.x -= state.airborneAccel;
            }
            else if (playerDist > 100)
            {
                phys.velocity.x += state.airborneAccel;
            }
            absClamp(phys.velocity.x, state.maxVelX);
        }

        if(ai.stateTimer.Tick(deltaTime))
        {
            ai.stateTimer.Start(2.0f);
            generateAttack(id, AttackDir::Attack_Left, AttackType::Airborne);
            ai.attackTimer.Start(0.5f);

            phys.velocity.y = -state.jumpVelocity;
            state.airborne = true;

            if(playerDist < 0)
            {
                phys.velocity.x -= state.jumpVelocity/2;
            }
            else
            {
                phys.velocity.x += state.jumpVelocity/2;
            }
        }

        if(!ai.attackTimer.Tick(deltaTime))
        {
            pos.rotation += 0.3f;
        }
        else
        {
            pos.rotation = 0;
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

//shoots at the player
//tries to jump up to get a good shot.
void GameLogic::LaserAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();
    AIComponent& ai = *enemy->getComponent<AIComponent>();
    
    Vector2 playerPos = playerObj->getComponent<PositionalComponent>()->position;
    float playerDist =  playerPos.x - pos.position.x;

    if(ai.attackTimer.Tick(deltaTime))
    {
        
        if(playerDist < 0)
            generateAttack(enemy->id, AttackDir::Attack_Left, AttackType::Laser);
        else
            generateAttack(enemy->id, AttackDir::Attack_Right, AttackType::Laser);

        ai.attackTimer.Start(4.0f);

    }

    if(!state.airborne && playerPos.y < pos.position.y)
    {
        phys.velocity.y = -state.jumpVelocity;
        state.airborne = true;
    }

    if(!state.knocked)
    {
        if(!state.airborne)
            phys.velocity.x = 0;
        float distCenter = pos.position.x - level.LevelPos.x;
        if( distCenter > 350)
        {
            phys.velocity.x -= state.airborneAccel;
        }
        else if(distCenter < -350)
        {
            phys.velocity.x += state.airborneAccel;
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

//runs back and forth.
//When it stops, it spins for a bit, then explodes
//will not explode unless on the ground.
//has a tendency to fly around wildly when knocked
void GameLogic::ExplodingAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();
    AIComponent& ai = *enemy->getComponent<AIComponent>();

    if(ai.outerState == AIOuterState::Punch)
    {
        pos.rotation += ai.stateTimer.passed/10;
    }

    if(state.knocked)
    {
        if(state.knockedTimer.Tick(deltaTime))
        {
            state.knocked = false;
        }
        return;
    }

    ai.stateTimer.Tick(deltaTime);

    Vector2 playerPos = playerObj->getComponent<PositionalComponent>()->position;
    float playerDist =  playerPos.x - pos.position.x;

    if(!state.airborne)
    {
        phys.velocity.x = 0;
        Vector2 newPosition;
        switch(ai.outerState)
        {
        case Move:
            switch(ai.innerState)
            {
            case MoveLeft:
                newPosition = pos.position;
                newPosition.x -= state.movementSpeed/2;
                if(level.IsOnPlatform(newPosition, phys.body))
                {
                    pos.position = newPosition;
                }

                if(ai.stateTimer.IsDone())
                {
                    ai.outerState = Punch;
                    ai.stateTimer.Start(5.0f);
                }
                break;

            case MoveRight:
                newPosition = pos.position;
                newPosition.x += state.movementSpeed/2;
                if(level.IsOnPlatform(newPosition, phys.body))
                {
                    pos.position = newPosition;
                }

                if(ai.stateTimer.IsDone())
                {
                    ai.outerState = Punch;
                    ai.stateTimer.Start(5.0f);
                }
                break;
            }
            break;
        case Punch:
            if(ai.stateTimer.IsDone())
            {
                generateAttack(enemy->id,AttackDir::Attack_Left, AttackType::Explosion);
                ai.outerState = Wait;
                ai.stateTimer.Start(2.0f);
                pos.rotation = 0;
            }
            break;

        case Wait:
            if(ai.stateTimer.IsDone())
            {
                ai.outerState = Move;
                ai.innerState = ai.innerState == MoveLeft ? MoveRight : MoveLeft;
                ai.stateTimer.Start(3.0f);
            }
            
            break;
        }
    }
    else
    {
        if(state.jumpCount < 5 && state.jumpTimer.Tick(deltaTime))
        {
            phys.velocity.y = -state.jumpVelocity/1.5f;
            ++state.jumpCount;
            state.jumpTimer.Start(0.5f);
        }
        float centerDir = level.LevelPos.x - pos.position.x;
        if(centerDir < 0)
        {
            phys.velocity.x -= state.airborneAccel;
            pos.position.x -= state.movementSpeed/10;
        }
        else
        {
            phys.velocity.x += state.airborneAccel;
            pos.position.x += state.movementSpeed/10;
        }
    }

    
}

////////////////////////////////
// Attack Object Generation
////////////////////////////////

void makePunch(PhysicsComponent* atkPhys)
{
    BB_Rectangle* attackBody = new BB_Rectangle();
    attackBody->extents = Vector2(10,10);
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = true;
    atkPhys->IsGhost = true;   
}

void makeKick(PhysicsComponent* atkPhys)
{
    BB_Rectangle* attackBody = new BB_Rectangle();
    attackBody->extents = Vector2(70,10);
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = true;
    atkPhys->IsGhost = true;   
}

void makeSpin(PhysicsComponent* atkPhys, float radius)
{
    BB_Circle* attackBody = new BB_Circle();
    attackBody->radius = radius;
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = true;
    atkPhys->IsGhost = true;   
}

void makeLaser(PhysicsComponent* atkPhys)
{
    BB_Circle* attackBody = new BB_Circle();
    attackBody->radius = 10.0f;
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = false;
    atkPhys->IsGhost = true;   
}

void GameLogic::generateAttack(int ownerID, AttackDir aDir, AttackType aType)
{
    ComponentFactory& CF = ComponentFactory::Instance();

    GameObject* attackObj = GameObjectCache::Instance().Create();
    GameObject* owner = GameObjectCache::Instance().Get(ownerID);
    PhysicsComponent* ownerPhys = owner->getComponent<PhysicsComponent>();

    AttackComponent* atkC = CF.createComponent<AttackComponent>();
    atkC->direction = aDir;
    atkC->type = aType;
    atkC->ownerID = ownerID;

    PositionalComponent* atkPos = CF.createComponent<PositionalComponent>();
    atkPos->position = owner->getComponent<PositionalComponent>()->position;
    Vector2 ownerDim;
    
    if( ownerPhys->body->type == BB_Type::Rect)
    {
        BB_Rectangle* rect = static_cast<BB_Rectangle*>(ownerPhys->body);
        ownerDim = rect->extents;
    }

    PhysicsComponent* atkPhys = CF.createComponent<PhysicsComponent>();
    
    if(aType == AttackType::Regular)
    {
        if(aDir == AttackDir::Attack_Left)
        {
            makePunch(atkPhys);
            atkPhys->offset = Vector2(-ownerDim.x/2 - 35, 0);
            atkC->attackTimer.Start(0.1f);
            atkC->damage = 5;
        }
        else if(aDir == AttackDir::Attack_Right)
        {
            makePunch(atkPhys);
            atkPhys->offset = Vector2(ownerDim.x/2 + 35, 0);
            atkC->attackTimer.Start(0.1f);
            atkC->damage = 5;
        }
        else if(aDir == AttackDir::Attack_Up)
        {
            makePunch(atkPhys);
            atkPhys->offset = Vector2(0, -ownerDim.y/2 - 35);
            atkC->attackTimer.Start(0.1f);
            atkC->damage = 5;
        }
        else if(aDir == AttackDir::Attack_Down)
        {
            makeKick(atkPhys);
            atkPhys->offset = Vector2(0, ownerDim.y/2);
            atkC->attackTimer.Start(0.1f);
            atkC->damage = 5;
        }
    }
    else if (aType == AttackType::Airborne)
    {
        makeSpin(atkPhys, ownerDim.x/1.5f);
        atkPhys->offset = Vector2(0, 0);
        atkC->attackTimer.Start(0.5f);
        atkC->damage = 10;
    }
    else if(aType == AttackType::Laser)
    {
        makeLaser(atkPhys);
        
        if(aDir == AttackDir::Attack_Left)
        {
            atkPos->position.x +=  -ownerDim.x/2 - 35;
            atkPhys->velocity = Vector2(-500, 0);
        }
        else
        {
            atkPos->position.x +=  ownerDim.x/2 + 35;
            atkPhys->velocity = Vector2(500, 0);
        }

        atkC->attackTimer.Start(3.0f);
        atkC->damage = 10;
    }
    else if(aType == AttackType::Explosion)
    {
        makeSpin(atkPhys, 100.0f);
        atkPhys->offset = Vector2(0, 0);
        atkC->attackTimer.Start(0.5f);
        atkC->damage = 30;
    }

    if(ownerID == playerObj->id)
    {
        atkPhys->registerCollideHandler<GameLogic, &GameLogic::PlayerAttackCollision>(this);
    }
    else
    {
        atkPhys->registerCollideHandler<GameLogic, &GameLogic::EnemyAttackCollision>(this);
    }
    
    attackObj->attachComponent(atkPos);
    attackObj->attachComponent(atkPhys);
    attackObj->attachComponent(atkC);
    CORE->BroadcastMessage(&AttachDebugDrawMessage(attackObj->id));

    attacks.push_back(attackObj);
}