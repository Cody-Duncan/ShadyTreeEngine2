#include "stdafx.h"
#include "PhysicsSystem.h"

#include "PhysicsComponent.h"
#include "PositionalComponent.h"
#include "ComponentFactory.h"
#include "GameObjectCache.h"
#include "BB.h"

#include "Contact.h"

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
    std::vector<PhysicsComponent>& phys         = CF.getCache<PhysicsComponent>()->storage;      //Static Physics components
    std::vector<PositionalComponent>& positions = CF.getCache<PositionalComponent>()->storage;   //Position Components
    GameObjectCache& GOC                        = GameObjectCache::Instance();

    std::vector<PhysicsComponent>::iterator A_iter;
    std::vector<PhysicsComponent>::iterator B_iter;

    std::vector<Contact> contacts;

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
                    contacts.push_back(c);
                }
            }
        }// End B loop
    }//End A loop
}// End Function