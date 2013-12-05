#include "stdafx.h"
#include "PhysicsSystem.h"

#include "PhysicsStaticComponent.h"
#include "PhysicsDynamicComponent.h"
#include "PositionalComponent.h"
#include "ComponentFactory.h"
#include "GameObjectCache.h"

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
    if(!CF.hasComponentCache<PhysicsStaticComponent>() || !CF.hasComponentCache<PhysicsDynamicComponent>() ) //check for any graphicsComponents
        return;

    //grab the caches
    std::vector<PhysicsStaticComponent>& statics = CF.getCache<PhysicsStaticComponent>()->storage;      //Static Physics components
    std::vector<PhysicsDynamicComponent>& dynamics = CF.getCache<PhysicsDynamicComponent>()->storage;   //Dynamic Physics components
    std::vector<PositionalComponent>& positions = CF.getCache<PositionalComponent>()->storage;          //Position Components
    GameObjectCache& GOC = GameObjectCache::Instance();                                                 

    //check for cache, set length to 0 to prevent drawing nothing.
    int staticsLength   = CF.hasComponentCache<PhysicsStaticComponent>()  ? statics.size()   : 0;
    int dynamicsLength  = CF.hasComponentCache<PhysicsDynamicComponent>() ? dynamics.size()  : 0;
    int positionsLength = CF.hasComponentCache<PositionalComponent>()     ? positions.size() : 0;

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
