#pragma once

#include "ISystem.h"
#include "Contact.h"

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

    ST_API void setGravity(float g);
    ST_API void generateDebugDraw();
    ST_API void AttachDebugDraw(int id);

private:
    float gravity;
    std::vector<Contact> contacts;

    void Integrate(float deltaTime);

    void DetectCollisions();

    void ResolveContacts(float deltaTime);

    void UpdateDebugDraw();

    void SendCollisionMessages();
    
};

