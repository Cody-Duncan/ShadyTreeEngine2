#pragma once

#include "GameObject.h"

class GameObjectCache
{
public:
    static GameObjectCache& Instance()
    {
        static GameObjectCache factory;
        return factory;
    }

    std::vector<GameObject> entities;
    std::list<int> freeSlots;

    void Reserve(int size);

    GameObject* Create();
    GameObject* Get(int id);

    void DestroyNow(int id);
    
    void DestroyLater(int id);
    void DestroyCleanup();

    void Clear();

private:
    GameObjectCache(void);
    ~GameObjectCache(void);
    GameObjectCache(GameObjectCache& rhs);
    GameObjectCache& operator=(GameObjectCache& rhs);

    void deactivateGameObject(int id);
    void deactivateGameObject(GameObject& go);

    static int genID;
    int generateID();

    std::unordered_map<int, int> ID_Index;
    std::list<int> cleanupList;

};

