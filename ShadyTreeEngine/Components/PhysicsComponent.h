#pragma once
#include "GameObject\Component.h"
#include "Resources\GraphicsResourceHandles.h"
#include "Math\MathLib.h"
#include "GameObject\ComponentTypeID.h"
#include "Physics\BB.h"
#include "DelegateFunc.h"
#include "Messaging\ContactMessage.h"

class PhysicsComponent : public Component
{
public:
    ST_API PhysicsComponent(void);
    ST_API PhysicsComponent(int _id, bool _active);
    ST_API ~PhysicsComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    bool IsStatic;
    bool IsGhost;

    float Mass;
    float InvMass;
    float Restitution;

    Vector2 velocity;
    Vector2 acceleration;
    Vector2 force;
    Vector2 offset;

    BB* body;

    std::list<DelegateFunc> collisionDelegates;

    template <class T, void (T::*TMethod)(Message*)>
    void registerCollideHandler(T* object);
    ST_API void clearCollideHandlers();
    void CollideEvent(ContactMessage* m);

private:
    void InitialValues();
};


RegisterType(PhysicsComponent)

template <class T, void (T::*TMethod)(Message*)>
void PhysicsComponent::registerCollideHandler(T* object)
{
    collisionDelegates.push_back(DelegateFunc::from_method<T, TMethod>(object));
}