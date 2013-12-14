#pragma once
#include "Component.h"
#include "GraphicsResourceHandles.h"
#include "MathLib.h"
#include "ComponentTypeID.h"
#include "BB.h"
#include "DelegateFunc.h"
#include "ContactMessage.h"

class PhysicsComponent : public Component
{
public:
    ST_API PhysicsComponent(void);
    ST_API PhysicsComponent(int _id, bool _active);
    ST_API ~PhysicsComponent(void);

    ST_API virtual void CloneFrom(Component* c);

    bool IsStatic;

    float Mass;
    float InvMass;
    float Restitution;

    Vector2 velocity;
    Vector2 acceleration;
    Vector2 force;

    BB* body;

    DelegateFunc collisionDelegate;

    template <class T, void (T::*TMethod)(Message*)>
    void registerCollideHandler(T* object);

    void CollideEvent(ContactMessage* m);

private:
    void InitialValues();
};


RegisterType(PhysicsComponent)

template <class T, void (T::*TMethod)(Message*)>
void PhysicsComponent::registerCollideHandler(T* object)
{
    collisionDelegate = DelegateFunc::from_method<T, TMethod>(object);
}