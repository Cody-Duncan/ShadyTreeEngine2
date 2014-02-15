#include "AIComponent.h"
AIComponent::AIComponent(void)
{
    Initialize();
}

AIComponent::AIComponent(int _id, bool _active)
{
    Initialize();
}

void AIComponent::Initialize()
{
    outerState = 0;
    innerState = 0;
}

void AIComponent::CloneFrom(Component* _c)
{
    AIComponent* c = dynamic_cast<AIComponent*>(_c);
    aiType = c->aiType;
}