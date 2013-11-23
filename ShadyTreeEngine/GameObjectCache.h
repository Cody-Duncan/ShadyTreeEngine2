#pragma once

#include "GameObject.h"

class GameObjectCache
{
public:
    ST_API static GameObjectCache& Instance();

    std::vector<GameObject> entities;
    std::list<int> freeSlots;

    ST_API void Reserve(int size);

    ST_API GameObject* Create();
    ST_API GameObject* Get(int id);

    ST_API void DestroyNow(int id);
    
    ST_API void DestroyLater(int id);
    ST_API void DestroyCleanup();

    ST_API void Clear();

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

