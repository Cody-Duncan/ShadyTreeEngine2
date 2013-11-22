#pragma once

#include "GameObjectCache.h"

class GameObjectFactory
{
public:
    static GameObjectFactory& Instance()
    {
        static GameObjectFactory factory;
        return factory;
    }

    GameObject* createGraphicalEntity();

private:
    GameObjectFactory(void);
    ~GameObjectFactory(void);
    GameObjectFactory( GameObjectFactory& rhs);
    GameObjectFactory& operator=(GameObjectFactory& rhs);
};

