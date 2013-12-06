#pragma once

#include "ISystem.h"

class PhysicsSystem : public ISystem
{
public:
    ST_API PhysicsSystem(void);
    ~PhysicsSystem(void);

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();
    virtual void Free();
    virtual void RecieveMessage(Message* msg);

    void setGravity(float g);

private:
    float gravity;

    void Integrate(float deltaTime);

    void DetectCollisions();
};
