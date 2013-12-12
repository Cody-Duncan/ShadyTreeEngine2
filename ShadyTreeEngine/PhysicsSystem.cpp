#include "stdafx.h"
#include "PhysicsSystem.h"

#include "PhysicsComponent.h"
#include "PositionalComponent.h"
#include "ComponentFactory.h"
#include "GameObjectCache.h"
#include "BB.h"

#include "Contact.h"


#include "DebugDrawComponent.h"

PhysicsSystem::PhysicsSystem(void)
{
}


PhysicsSystem::~PhysicsSystem(void)
{
}

void PhysicsSystem::Init()
{
    gravity = 1.0f;
}

void PhysicsSystem::Load()
{
    
}

void PhysicsSystem::Update(float deltaTime)
{
    Integrate(deltaTime);
    DetectCollisions();
    ResolveContacts(deltaTime);

    contacts.clear();
}

void PhysicsSystem::Unload()
{
    
}

void PhysicsSystem::Free()
{
    
}

void PhysicsSystem::RecieveMessage(Message* msg)
{
    
}

void PhysicsSystem::setGravity(float g)
{
    gravity = g;
}


void PhysicsSystem::Integrate(float deltaTime)
{
    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<PhysicsComponent>() || !CF.hasComponentCache<PositionalComponent>() ) //check for any graphicsComponents
        return;

    //grab the caches
    std::vector<PhysicsComponent>& phys         = CF.getCache<PhysicsComponent>()->storage;      //Static Physics components
    std::vector<PositionalComponent>& positions = CF.getCache<PositionalComponent>()->storage;   //Position Components
    GameObjectCache& GOC                        = GameObjectCache::Instance();                                                 

    //Integrate
    for(unsigned int i = 0; i < phys.size(); ++i)
    {
        PhysicsComponent& phy  = phys[i];
        if(phy.IsStatic)
            continue;

        PositionalComponent& pos = *GOC.Get(phy.parentID)->getComponent<PositionalComponent>();

        //gravity
        phy.acceleration.y += gravity;
        
        //Integrate position
        pos.position = pos.position + phy.velocity * deltaTime;

        //Integrate velocity with force
        Vector2 newAcc = phy.force * phy.InvMass + phy.acceleration;
        phy.velocity = phy.velocity + newAcc * deltaTime;

        //Zero force
        phy.force = Vector2();
    }
}

/// <summary>
/// Detects the collisions.
/// </summary>
void PhysicsSystem::DetectCollisions()
{
    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<PhysicsComponent>() || !CF.hasComponentCache<PositionalComponent>() ) //check for any graphicsComponents
        return;

    //grab the caches
    std::vector<PhysicsComponent>& phys         = CF.getCache<PhysicsComponent>()->storage;      //Physics components
    std::vector<PositionalComponent>& positions = CF.getCache<PositionalComponent>()->storage;   //Position Components
    GameObjectCache& GOC                        = GameObjectCache::Instance();

    std::vector<PhysicsComponent>::iterator A_iter;
    std::vector<PhysicsComponent>::iterator B_iter;

    

    for(A_iter = phys.begin(); A_iter != phys.end(); ++A_iter)
    {
        PhysicsComponent& A = *A_iter;
        PositionalComponent& posA = *A_iter->parent()->getComponent<PositionalComponent>();

        if(!A.active)
            continue;

        B_iter = A_iter;
        ++B_iter;
        for( ; B_iter != phys.end(); ++B_iter)
        {
            PhysicsComponent& B = *B_iter;
            PositionalComponent& posB = *B_iter->parent()->getComponent<PositionalComponent>();

            if(!B.active)
            continue;

            if( !A.IsStatic || !B.IsStatic )
            {
                Contact c;
                if(CollisionCheck(A_iter->body, posA.position, B_iter->body, posB.position, c))
                {
                    c.ObjIDs[0]     = A.parentID;
                    c.ObjIDs[1]     = B.parentID;
                    c.Velocities[0] = A.velocity;
                    c.Velocities[1] = B.velocity;
                    c.Restitution = min(A.Restitution,B.Restitution);
                    contacts.push_back(c);
                }
            }
        }// End B loop
    }//End A loop
}// End Function

void ResolvePosition(Contact& c, 
    PhysicsComponent& APhys, PositionalComponent& APos,
    PhysicsComponent& BPhys, PositionalComponent& BPos)
{
    //calculate the penetration resolution, weighted by inverse mass
    // (higher mass == lower movement)
    float totalInverseMass = APhys.InvMass + BPhys.InvMass;
    Vector2 movePerInvMass = c.ContactNormal * (c.Penetration / totalInverseMass);
    c.Movement[0] =  movePerInvMass * APhys.InvMass;
    c.Movement[1] =  movePerInvMass * -BPhys.InvMass;

    //update the bodies' positions
    if(!APhys.IsStatic) APos.position += c.Movement[0];
    if(!BPhys.IsStatic) BPos.position += c.Movement[1];
}

inline float calcSeparatingVelocity(Vector2& AVel, Vector2& BVel, Vector2& ContactNormal)
{
    return (AVel- BVel).Dot( ContactNormal );
}

void ResolveVelocities(Contact& c, 
    PhysicsComponent& APhys, PositionalComponent& APos,
    PhysicsComponent& BPhys, PositionalComponent& BPos,
    float deltaTime)
{
    float SeparatingVelocity = calcSeparatingVelocity(APhys.velocity, BPhys.velocity, c.ContactNormal);

    if(SeparatingVelocity > 0) //not moving towards eachother
    {
        c.ContactImpulse = 0;
        return;
    }

    //reduce velocity by restitution
    float newSepVelocity = -SeparatingVelocity * c.Restitution;

    //determine what portion of separating velocity was caused by acceleration
    float AccelerationCausedSepVel = calcSeparatingVelocity(APhys.acceleration, BPhys.acceleration, c.ContactNormal) * deltaTime;

    if(AccelerationCausedSepVel < 0)
    {
        //remove closing velocity caused by acceleration buildup
        newSepVelocity += c.Restitution * AccelerationCausedSepVel;
        if(newSepVelocity < 0) newSepVelocity = 0;
    }

    //calculate:
    //change in velocity/(aMass + bMass) == impulse (force)
    float deltaV = newSepVelocity - SeparatingVelocity;
    float totalInverseMass = APhys.InvMass + BPhys.InvMass;
    float impulse = deltaV / totalInverseMass;

    c.ContactImpulse = impulse;

    Vector2 impulsePerInvMass = c.ContactNormal * impulse;

    //Apply impulse in direction of contact
    APhys.velocity = APhys.velocity + impulsePerInvMass * APhys.InvMass;
    BPhys.velocity = BPhys.velocity + impulsePerInvMass * -BPhys.InvMass;
}

void PhysicsSystem::ResolveContacts(float deltaTime)
{
     if(contacts.size() == 0)
        return;

    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<PhysicsComponent>() || !CF.hasComponentCache<PositionalComponent>() ) //check for any graphicsComponents
        return;
    GameObjectCache& GOC = GameObjectCache::Instance();

    for (unsigned i=0; i < contacts.size(); i++)
    {
        Contact& c = contacts[i];

        GameObject* A = GOC.Get(c.ObjIDs[0]);
        GameObject* B = GOC.Get(c.ObjIDs[1]);

        PhysicsComponent& APhys = *A->getComponent<PhysicsComponent>();
        PositionalComponent& APos = *A->getComponent<PositionalComponent>();
        PhysicsComponent& BPhys = *B->getComponent<PhysicsComponent>();
        PositionalComponent& BPos = *B->getComponent<PositionalComponent>();

        ResolvePosition(c, APhys, APos, BPhys, BPos);
        ResolveVelocities(c, APhys, APos, BPhys, BPos, deltaTime);
    }
}

void PhysicsSystem::generateDebugDraw()
{
    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<PhysicsComponent>() || !CF.hasComponentCache<PositionalComponent>() ) //check for any graphicsComponents
        return;
    //grab the caches
    std::vector<PhysicsComponent>& phys         = CF.getCache<PhysicsComponent>()->storage;      //Physics components
    std::vector<PositionalComponent>& positions = CF.getCache<PositionalComponent>()->storage;   //Position Components
    GameObjectCache& GOC                        = GameObjectCache::Instance();

    for(unsigned int i = 0; i < GOC.entities.size(); ++i)
    {
        GameObject& go = GOC.entities[i];
        if(!go.active || !go.getComponent<PhysicsComponent>() || go.hasComponent<DebugDrawComponent>())
            continue;

        DebugDrawComponent* newDDC = CF.createComponent<DebugDrawComponent>();
        PhysicsComponent* phys = go.getComponent<PhysicsComponent>();

        newDDC->lines.push_back(Vector2(0,0));
        newDDC->lines.push_back(phys->velocity);

        phys->body->generateGeometry(newDDC->geometry);

        newDDC->active = go.active;

        go.attachComponent(newDDC);

    }
}