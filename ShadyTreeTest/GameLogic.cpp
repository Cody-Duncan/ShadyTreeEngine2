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
#include "PrimitiveGraphicsComponent.h"
#include "PowerupComponent.h"

#include "DeSerializer.h"
#include "Messenger.h"
#include "ContactMessage.h"
#include "Timer.h"
#include "ShadyTreeEngine.h"
#include "DebugDrawMessage.h"
#include "ChangeGravityMessage.h"
#include "ChangeStateMessage.h"

#include <time.h>
#include <random>


#define PLAYER_LIVES_NUM 3
#define POWERUP_DROP_PERIOD 7.0f
#define WIN_TIMER_DURATION 3.0f

#define HIT_DIRECTION_ANGLE 0.7f
#define HIT_VELOCITY_MULTIPLIER 1500

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

    powerMap["Heal"] = &GameLogic::HealUp;
    powerMap["Whirl"] = &GameLogic::WhirlUp;
    powerMap["Jump"] = &GameLogic::JumpUp;
    powerMap["Speed"] = &GameLogic::SpeedUp;
    powerMap["Boom"] = &GameLogic::BoomUp;
}

void GameLogic::Load()
{
    DebugPrintf("GAME: Loading Game Logic\n");

    GameObjectFactory& GOF = GameObjectFactory::Instance();
    
    //Build Level
    DeSerializer s;
    level.Initialize("ArenaLevel", s, Height, Width, 1.5f);

    //build player
    s.BuildArchetypes("Archetypes");
    s.BuildSubtypes("subtypes");
    s.BuildPowerupTypes("powerupTypes");

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
    winImage = GOF.createGraphicalEntity("WinMessage", Vector2((float)Width/2, (float)Height/2));
    winImage->getComponent<GraphicsComponent>()->active = false;

    Resources::Instance().LoadTextureRes("LoseMessage");
    loseImage = GOF.createGraphicalEntity("LoseMessage", Vector2((float)Width/2, (float)Height/2));
    loseImage->getComponent<GraphicsComponent>()->active = false;

    winState = false;
    loseState = false;

    PlayerLives = PLAYER_LIVES_NUM;
    powerupDropPeriod = POWERUP_DROP_PERIOD;
    powerUpDropTimer.Start(powerupDropPeriod);
    duration = 0;
    srand ((unsigned int)time(NULL));
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

    duration += deltaTime;

    //Check Win/Lose conditions
    if(enemies.size() == 0 && !winState && !loseState)
    {
        winState = true;
        winTimer.Start(WIN_TIMER_DURATION);
        winImage->getComponent<GraphicsComponent>()->active = true;
    }
    if(PlayerLives <= 0 && !winState && !loseState)
    {
        loseState = true;
        winTimer.Start(WIN_TIMER_DURATION);
        loseImage->getComponent<GraphicsComponent>()->active = true;

        //delete the enemies
        auto enemyI = enemies.begin();
        while( enemyI != enemies.end() )
        {
            GOC.DestroyLater((*enemyI)->id);
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
                phys.velocity.y = -state.getJumpVelocity();
                ++state.jumpCount;
            }

        }
        else // on ground
        {
            phys.velocity.x = 0;
            float lastXPos = pos.position.x;

            if(state.attackWait.IsDone()) //pause movement when attacking.
            {
                if(gINPUTSTATE->keyDown(VK_LEFT))
                {
                    pos.position.x -= state.getMovementSpeed();
                }
                if(gINPUTSTATE->keyDown(VK_RIGHT))
                {
                    pos.position.x += state.getMovementSpeed();
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
                    phys.velocity.y -= state.getJumpVelocity();
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

    }
    else //do nothing if knocked, player loses control momentarily
    {
        if(state.knockedTimer.IsDone())
        {
            state.knocked = false;
        }
    }

    
    if(gINPUTSTATE->keyPressed(VK_BACK)) //back to intro screen
    {
        CORE->BroadcastMessage(&ChangeStateMessage(State::IntroState));
    }
    if(gINPUTSTATE->keyPressed(VK_RETURN)) //back to intro screen
    {
        CORE->BroadcastMessage(&ToggleDebugDrawMessage());
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
            GOC.DestroyLater((*enemyI)->id);
            enemies.erase(enemyI++);
        }
        else
        {
            ++enemyI;
        }
    }

    //update attack objects
    auto attackI = attacks.begin();
    while( attackI != attacks.end() )
    {
        GameObject* go = *attackI;
        if(go->id == INACTIVE_ID)
        {
            attacks.erase(attackI++);
            continue;
        }

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
            GOC.DestroyLater(atk->parent()->id);
            attacks.erase(attackI++);
        }
        else
        {
            ++attackI;
        }
    }

    powerUpDropTimer.Tick(deltaTime);
    if(powerUpDropTimer.IsDone())
    {
        generatePowerUp();
        powerUpDropTimer.Start(powerupDropPeriod);
    }

    auto powerI = powerups.begin();
    while( powerI != powerups.end() )
    {
        GameObject* go = *powerI;
        PowerupComponent* pwrup = go->getComponent<PowerupComponent>();
        pwrup->spawntimer.Tick(deltaTime);

        if(pwrup->spawntimer.IsDone())
        {
            GOC.DestroyLater(pwrup->parent()->id);
            powerups.erase(powerI++);
        }
        else
        {
            ++powerI;
        }
    }

    if(playerObj->hasComponent<PowerupComponent>())
    {
        PowerupComponent* pwrup = playerObj->getComponent<PowerupComponent>();
        (this->*powerMap[pwrup->powerType])(deltaTime, playerObj->id);
    }

    // Draw some debug information.
    char buf[100];
    sprintf_s(buf, "Duration: %8.2f", duration);
    CORE->BroadcastMessage( &DrawTextMessage(std::string(buf) ,Vector2(1,16)) );
    sprintf_s(buf, "Player Damage: %i%%", state.damage);
    CORE->BroadcastMessage( &DrawTextMessage( buf ,Vector2(1,32)) );
    sprintf_s(buf, "Player Position: %6.2f, %6.2f", pos.position.x, pos.position.y);
    CORE->BroadcastMessage( &DrawTextMessage( buf ,Vector2(1,48)) );
}

void GameLogic::Unload()
{
    playerObj = nullptr;
    enemies.clear();
    attacks.clear();
    level.Dispose();

    GameObjectFactory& GOF = GameObjectFactory::Instance();
    GOF.clearSubtypes();
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
    hitDir.y -= HIT_DIRECTION_ANGLE;
    phys.velocity += hitDir * (state.damage/100.0f) * HIT_VELOCITY_MULTIPLIER;
    state.airborne = true;
}


void removePowerUp(GameObject* go);
void givePowerUp(GameObject* go, GameObject* powerupObj);


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
        GameObjectCache::Instance().DestroyLater(attack->id);     //get rid of the attack object
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
        GameObjectCache::Instance().DestroyLater(attack->id);     //get rid of the attack object
        attacks.remove(attack);                                 //remove attack from attacks list.
    }
}

void GameLogic::PowerupCollision(Message* msg)
{
    ContactMessage cmsg = *(static_cast<ContactMessage*> (msg));

    GameObject* powerup = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[cmsg.recieverIndex]);
    GameObject* collideThing = GameObjectCache::Instance().Get(cmsg.contact.ObjIDs[(cmsg.recieverIndex + 1) % 2]);

    if(collideThing->id == playerObj->id)      //hit the player
    {

        //remove old powerup
        if(playerObj->hasComponent<PowerupComponent>())
        {
            removePowerUp(playerObj);
        }

        //give new one
        givePowerUp(playerObj, powerup);

        //destroy the powerup object.
        GameObjectCache::Instance().DestroyLater(powerup->id);
        powerups.remove(powerup);
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
            //airborne moves via acceleration
            if(playerDist < 0) //player is left, move left
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
            //ground movement is by static movement
            phys.velocity.x = 0;
            if(playerDist < 0)
            {
                pos.position.x -= state.getMovementSpeed()/2;
            }
            else
            {
                pos.position.x += state.getMovementSpeed()/2;
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

#define PUNCH_AI_MOVE_PERIOD 2.5f
#define PUNCH_AI_PUNCH_DISTANCE 70

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

    //punch if close to the player
    Vector2 playerPos = playerObj->getComponent<PositionalComponent>()->position;
    float playerDist =  playerPos.x - pos.position.x;
    if(abs(playerDist) < PUNCH_AI_PUNCH_DISTANCE)
        ai.outerState = Punch;

    //this ai is designed to move one direction, stopping at the edge of a platform.
    //after that, wait
    //then punch and begiin moving again.
    //also punches wildly if player is nearby, which also changes direction
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
                newPosition.x -= state.getMovementSpeed()/2;
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
                newPosition.x += state.getMovementSpeed()/2;
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
            ai.stateTimer.Start(PUNCH_AI_MOVE_PERIOD);
            
            break;
        
        }
    }
    else
    {
        //moves toward the center if airborne
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

#define FLY_AI_JUMP_PERIOD 2.0f
#define FLY_AI_MOVE_RANGE 100

//jumps at the player and does a spin attack!
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
            //if airborne, move toward player
            if(playerDist < -FLY_AI_MOVE_RANGE)
            {
                phys.velocity.x -= state.airborneAccel;
            }
            else if (playerDist > FLY_AI_MOVE_RANGE)
            {
                phys.velocity.x += state.airborneAccel;
            }
            absClamp(phys.velocity.x, state.maxVelX);
        }

        if(ai.stateTimer.Tick(deltaTime))
        {
            //jump at and attack the player
            ai.stateTimer.Start(FLY_AI_JUMP_PERIOD);
            generateAttack(id, AttackDir::Attack_Left, AttackType::Spin);
            ai.attackTimer.Start(0.5f);

            phys.velocity.y = -state.getJumpVelocity();
            state.airborne = true;

            if(playerDist < 0)
            {
                phys.velocity.x -= state.getJumpVelocity()/2;
            }
            else
            {
                phys.velocity.x += state.getJumpVelocity()/2;
            }
        }
        
        //spin while attacking
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

#define LASER_AI_MOVE_RANGE 350
#define LASER_AI_FIRE_PERIOD 5.0f

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
        //I'M A' FIRING MA LASA' , BWAAAAAH! (I am firing my laser *fires laser*)
        if(playerDist < 0)
            generateAttack(enemy->id, AttackDir::Attack_Left, AttackType::Laser);
        else
            generateAttack(enemy->id, AttackDir::Attack_Right, AttackType::Laser);

        ai.attackTimer.Start(LASER_AI_FIRE_PERIOD);

    }

    //jump if player is higher
    if(!state.airborne && playerPos.y < pos.position.y)
    {
        phys.velocity.y = -state.getJumpVelocity();
        state.airborne = true;
    }

    if(!state.knocked)
    {
        //move back to the center if knocked too far away
        if(!state.airborne)
            phys.velocity.x = 0;
        float distCenter = pos.position.x - level.LevelPos.x;
        if( distCenter > LASER_AI_MOVE_RANGE)
        {
            phys.velocity.x -= state.airborneAccel;
        }
        else if(distCenter < -LASER_AI_MOVE_RANGE)
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

#define EXPLODE_AI_MOVE_PERIOD 5.0f
#define EXPLODE_AI_WAIT_PERIOD 3.0f
#define EXPLODE_AI_PUNCH_PERIOD 2.0f
#define EXPLODE_AI_JUMP_PERIOD 0.5f

//runs back and forth.
//When it stops, it spins for a bit, then explodes
//will not explode unless on the ground.
//has a tendency to fly around wildly when knocked because it has a quintuple jump
void GameLogic::ExplodingAI(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* enemy = GOC.Get(id);
    PositionalComponent& pos    = *enemy->getComponent<PositionalComponent>();
    PhysicsComponent& phys      = *enemy->getComponent<PhysicsComponent>();
    PlayerStateComponent& state = *enemy->getComponent<PlayerStateComponent>();
    AIComponent& ai = *enemy->getComponent<AIComponent>();

    //in this context, Punch == Getting ready to explode

    //spin while readying
    //spin faster as it gets closer
    if(ai.outerState == AIOuterState::Punch)
    {
        pos.rotation += ai.stateTimer.passed/10;
    }

    //do nothing if knocked
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
        //when on ground, move a direction, stop, spin, explode
        //then go  the other way and repeat
        phys.velocity.x = 0;
        Vector2 newPosition;
        switch(ai.outerState)
        {
        case Move:
            switch(ai.innerState)
            {
            case MoveLeft:
                newPosition = pos.position;
                newPosition.x -= state.getMovementSpeed()/2;
                if(level.IsOnPlatform(newPosition, phys.body))
                {
                    pos.position = newPosition;
                }

                if(ai.stateTimer.IsDone())
                {
                    ai.outerState = Punch;
                    ai.stateTimer.Start(EXPLODE_AI_MOVE_PERIOD);
                }
                break;

            case MoveRight:
                newPosition = pos.position;
                newPosition.x += state.getMovementSpeed()/2;
                if(level.IsOnPlatform(newPosition, phys.body))
                {
                    pos.position = newPosition;
                }

                if(ai.stateTimer.IsDone())
                {
                    ai.outerState = Punch;
                    ai.stateTimer.Start(EXPLODE_AI_MOVE_PERIOD);
                }
                break;
            }
            break;
        case Punch:
            if(ai.stateTimer.IsDone()) //Wait for it
            {
                generateAttack(enemy->id,AttackDir::Attack_Left, AttackType::Explosion); //BOOM
                ai.outerState = Wait;
                ai.stateTimer.Start(EXPLODE_AI_PUNCH_PERIOD);
                pos.rotation = 0;
            }
            break;

        case Wait:
            if(ai.stateTimer.IsDone())
            {
                ai.outerState = Move;
                ai.innerState = ai.innerState == MoveLeft ? MoveRight : MoveLeft;
                ai.stateTimer.Start(EXPLODE_AI_WAIT_PERIOD);
            }
            
            break;
        }
    }
    else
    {
        //quintuple jump
        if(state.jumpCount < 5 && state.jumpTimer.Tick(deltaTime))
        {
            phys.velocity.y = -state.getJumpVelocity()/1.5f;
            ++state.jumpCount;
            state.jumpTimer.Start(EXPLODE_AI_JUMP_PERIOD);
        }

        //moves toward the center if airborne
        //added some static movement to make it floaty
        float centerDir = level.LevelPos.x - pos.position.x;
        if(centerDir < 0)
        {
            phys.velocity.x -= state.airborneAccel;
            pos.position.x -= state.getMovementSpeed()/10;
        }
        else
        {
            phys.velocity.x += state.airborneAccel;
            pos.position.x += state.getMovementSpeed()/10;
        }
    }

    
}






////////////////////////////////
// Attack Object Generation
////////////////////////////////

#define PUNCH_DAMAGE 5
#define EXPLOSION_DAMAGE 30
#define LASER_DAMAGE 10
#define SPIN_DAMAGE 10
#define LASER_ATTACK_DURATION 1.0f

void makePunch(PhysicsComponent* atkPhys, PrimitiveGraphicsComponent* atkG)
{
    BB_Rectangle* attackBody = new BB_Rectangle();
    attackBody->extents = Vector2(10,10);
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = true;
    atkPhys->IsGhost = true;   

    atkPhys->body->generateGeometry(atkG->triangleListPoints);
    atkG->layer = 5;
}

void makeKick(PhysicsComponent* atkPhys,  PrimitiveGraphicsComponent* atkG)
{
    BB_Rectangle* attackBody = new BB_Rectangle();
    attackBody->extents = Vector2(70,10);
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = true;
    atkPhys->IsGhost = true;   

    atkPhys->body->generateGeometry(atkG->triangleListPoints);
    atkG->layer = 5;
}

void makeSpin(PhysicsComponent* atkPhys, float radius,  PrimitiveGraphicsComponent* atkG)
{
    BB_Circle* attackBody = new BB_Circle();
    attackBody->radius = radius;
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = true;
    atkPhys->IsGhost = true;  

    atkPhys->body->generateGeometry(atkG->triangleListPoints);
    atkG->layer = 5;
}

void makeLaser(PhysicsComponent* atkPhys,  PrimitiveGraphicsComponent* atkG)
{
    BB_Circle* attackBody = new BB_Circle();
    attackBody->radius = 10.0f;
    atkPhys->body = attackBody;
    atkPhys->Mass = 1.0f;
    atkPhys->InvMass = 1.0f;
    atkPhys->IsStatic = false;
    atkPhys->IsGhost = true;   

    atkPhys->body->generateGeometry(atkG->triangleListPoints);
    atkG->layer = 5;
}


void GameLogic::generateAttack(int ownerID, AttackDir aDir, AttackType aType)
{
    static Color gloveColor(1,0.5f,0.5f,1);
    static Color spinColor(1,1,1,0);
    static Color laserColor(0,0,1,1);
    static Color explosionColor(1,0,0,0.2f);
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
    atkPhys->clearCollideHandlers();
    PrimitiveGraphicsComponent* atkG = CF.createComponent<PrimitiveGraphicsComponent>();
    
    if(aType == AttackType::Regular)
    {
        if(aDir == AttackDir::Attack_Left)
        {
            makePunch(atkPhys, atkG);
            atkG->color = gloveColor;
            atkPhys->offset = Vector2(-ownerDim.x/2 - 35, 0);
            atkG->center = -atkPhys->offset;
            atkC->attackTimer.Start(0.1f);
            atkC->damage = PUNCH_DAMAGE;
        }
        else if(aDir == AttackDir::Attack_Right)
        {
            makePunch(atkPhys,atkG);
            atkG->color = gloveColor;
            atkPhys->offset = Vector2(ownerDim.x/2 + 35, 0);
            atkG->center = -atkPhys->offset;
            atkC->attackTimer.Start(0.1f);
            atkC->damage = PUNCH_DAMAGE;
        }
        else if(aDir == AttackDir::Attack_Up)
        {
            makePunch(atkPhys, atkG);
            atkG->color = gloveColor;
            atkPhys->offset = Vector2(0, -ownerDim.y/2 - 35);
            atkG->center = -atkPhys->offset;
            atkC->attackTimer.Start(0.1f);
            atkC->damage = PUNCH_DAMAGE;
        }
        else if(aDir == AttackDir::Attack_Down)
        {
            makeKick(atkPhys, atkG);
            atkG->color = gloveColor;
            atkPhys->offset = Vector2(0, ownerDim.y/2);
            atkG->center = -atkPhys->offset;
            atkC->attackTimer.Start(0.1f);
            atkC->damage = PUNCH_DAMAGE;
        }
    }
    else if (aType == AttackType::Spin)
    {
        makeSpin(atkPhys, ownerDim.x/1.5f, atkG);
        atkG->color = spinColor;
        atkPhys->offset = Vector2(0, 0);
        atkC->attackTimer.Start(0.5f);
        atkC->damage = SPIN_DAMAGE;
    }
    else if(aType == AttackType::Laser)
    {
        makeLaser(atkPhys, atkG);
        atkG->color = laserColor;
        
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

        atkC->attackTimer.Start(LASER_ATTACK_DURATION);
        atkC->damage = LASER_DAMAGE;
    }
    else if(aType == AttackType::Explosion)
    {
        makeSpin(atkPhys, 100.0f, atkG);
        atkG->color = explosionColor;
        atkPhys->offset = Vector2(0, 0);
        atkC->attackTimer.Start(0.5f);
        atkC->damage = EXPLOSION_DAMAGE;
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
    attackObj->attachComponent(atkG);
    attackObj->attachComponent(atkC);
    CORE->BroadcastMessage(&AttachDebugDrawMessage(attackObj->id));

    attacks.push_back(attackObj);
}







////////////////////////////////
// Powerup Generation
////////////////////////////////

#define SPIN_POWERUP_ATTACK_INTERVAL 0.25f
#define POWER_UP_DURATION 4.0f


void GameLogic::generatePowerUp()
{
    GameObjectFactory& GOF = GameObjectFactory::Instance();

    int randIndex = rand() % GOF.powerupTypes.size();
    std::string powerUpArchetype = GOF.powerupTypes[randIndex];
    GameObject* newPowerup = GameObjectFactory::Instance().cloneArchetype(powerUpArchetype);

    randIndex = rand() % level.enemyStartPositions.size();
    Vector2 dropSpot = level.enemyStartPositions[randIndex];
    dropSpot.y -= 10;

    PositionalComponent* pwrPos = newPowerup->getComponent<PositionalComponent>();
    pwrPos->position = dropSpot;

    PhysicsComponent* pwrPhys = newPowerup->getComponent<PhysicsComponent>();
    pwrPhys->registerCollideHandler<GameLogic, &GameLogic::PowerupCollision>(this);
}

void GameLogic::HealUp(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* powerOwner = GOC.Get(id);
    PlayerStateComponent& state = *powerOwner->getComponent<PlayerStateComponent>();
    PowerupComponent& pwrup     = *powerOwner->getComponent<PowerupComponent>();

    state.damage = 0;           //restore health
    removePowerUp(powerOwner);
}

void GameLogic::WhirlUp(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* powerOwner = GOC.Get(id);
    PositionalComponent& pos    = *powerOwner->getComponent<PositionalComponent>();
    PowerupComponent& pwrup     = *powerOwner->getComponent<PowerupComponent>();

    pos.rotation += 0.2f;   //spin

    pwrup.spawntimer.Tick(deltaTime);
    if(pwrup.spawntimer.IsDone())
    {
        //and attack
        generateAttack(powerOwner->id, AttackDir::Attack_Left, AttackType::Spin); 
        pwrup.spawntimer.Start(SPIN_POWERUP_ATTACK_INTERVAL);
    }
    
    pwrup.powerTimer.Tick(deltaTime);
    if(pwrup.powerTimer.IsDone())
    {
        removePowerUp(powerOwner);
    }
}

//just removes bonus after a time.
void GameLogic::JumpUp(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* powerOwner = GOC.Get(id);
    PlayerStateComponent& state = *powerOwner->getComponent<PlayerStateComponent>();
    PowerupComponent& pwrup     = *powerOwner->getComponent<PowerupComponent>();

    pwrup.powerTimer.Tick(deltaTime);
    if(pwrup.powerTimer.IsDone())
    {
        removePowerUp(powerOwner);
    }
}

//just removes bonus after a time.
void GameLogic::SpeedUp(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* powerOwner = GOC.Get(id);
    PlayerStateComponent& state = *powerOwner->getComponent<PlayerStateComponent>();
    PowerupComponent& pwrup     = *powerOwner->getComponent<PowerupComponent>();

    pwrup.powerTimer.Tick(deltaTime);
    if(pwrup.powerTimer.IsDone())
    {
        removePowerUp(powerOwner);
    }
}

void GameLogic::BoomUp(float deltaTime, int id)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GameObject* powerOwner = GOC.Get(id);

    //FWADOOM(explode)
    generateAttack(powerOwner->id, AttackDir::Attack_Left, AttackType::Explosion);
    
    removePowerUp(powerOwner);
}

void removePowerUp(GameObject* go)
{
    //remove all possible bonuses or effects of a powerup
    PlayerStateComponent* state = go->getComponent<PlayerStateComponent>();
    state->moveBonus = 0;
    state->jumpBonus = 0;

    PositionalComponent* pos = go->getComponent<PositionalComponent>();
    pos->rotation = 0;

    //remove the powerup
    go->removeComponent<PowerupComponent>();
}

void givePowerUp(GameObject* go, GameObject* powerupObj)
{
    //copy the powerup component to the player
    PowerupComponent* pwrC = powerupObj->getComponent<PowerupComponent>();
    PowerupComponent* player_pwrC = ComponentFactory::Instance().createComponent<PowerupComponent>();
    player_pwrC->powerType = pwrC->powerType;
    player_pwrC->powerTimer.Start(POWER_UP_DURATION);

    go->attachComponent(player_pwrC);

    if(player_pwrC->powerType.compare("Speed") == 0)
    {
        PlayerStateComponent* state = go->getComponent<PlayerStateComponent>();
        state->moveBonus = state->movementSpeed;
    }
    else if(player_pwrC->powerType.compare("Jump") == 0)
    {
        PlayerStateComponent* state = go->getComponent<PlayerStateComponent>();
        state->jumpBonus = state->jumpVelocity/3;
    }
}