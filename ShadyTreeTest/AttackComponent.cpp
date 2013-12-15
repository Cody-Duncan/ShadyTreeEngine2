#include "AttackComponent.h"

AttackComponent::AttackComponent(void)
{
    Initialize();
}

AttackComponent::AttackComponent(int _id, bool _active)
{
    Initialize();
}

void AttackComponent::Initialize()
{

}

void AttackComponent::CloneFrom(Component* _c)
{
    AttackComponent* c = dynamic_cast<AttackComponent*>(_c);
    ownerID = c->ownerID;
}