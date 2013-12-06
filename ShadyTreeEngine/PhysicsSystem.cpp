#include "stdafx.h"
#include "PhysicsSystem.h"

#include "PhysicsComponent.h"
#include "PositionalComponent.h"
#include "ComponentFactory.h"
#include "GameObjectCache.h"
#include "BB.h"

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
    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<PhysicsComponent>() ) //check for any graphicsComponents
        return;

    //grab the caches
    std::vector<PhysicsComponent>& phys         = CF.getCache<PhysicsComponent>()->storage;      //Static Physics components
    std::vector<PositionalComponent>& positions = CF.getCache<PositionalComponent>()->storage;   //Position Components
    GameObjectCache& GOC                        = GameObjectCache::Instance();                                                 

    //check for cache, set length to 0 to prevent drawing nothing.
    int physLength      = CF.hasComponentCache<PhysicsComponent>()    ? phys.size()      : 0;
    int positionsLength = CF.hasComponentCache<PositionalComponent>() ? positions.size() : 0;

    //Integrate

    BB_Rectangle br;

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
