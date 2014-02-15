#include "StdAfx.h"
#include "GameObject\GameObjectCache.h"
#include "Core\ShadyTreeEngine.h"

int GameObjectCache::genID = 0;


#define DEFAULT_NUM_OBJECTS 512
#define INACTIVE_GAMEOBJECT_ID -1


GameObjectCache& GameObjectCache::Instance()
{
    static GameObjectCache factory;
    return factory;
}

GameObjectCache::GameObjectCache(void)
{
    Reserve(DEFAULT_NUM_OBJECTS);
}


GameObjectCache::~GameObjectCache(void)
{
    entities.clear();
    freeSlots.clear();
}


void GameObjectCache::Reserve(int newSize)
{
    int currentSize = entities.size();
    entities.resize(newSize);
    for(int i = currentSize; i < newSize; ++i)
    {
        freeSlots.push_back(i);
        entities[i].active = false;
        entities[i].id = INACTIVE_GAMEOBJECT_ID;
    }
}


GameObject* GameObjectCache::Create()
{
    int index;
    int id;
    if(freeSlots.empty()) //use a free slot if available
    {
        index = entities.size();
        id = generateID();
        entities.emplace_back();
    }
    else //make a new slot, use it
    {
        index = freeSlots.front();
        id = generateID();
        freeSlots.pop_front();
    }

    ID_Index[id] = index;
    GameObject& go = entities[index];
    go.id = id;
    go.active = true;
    return &entities[index];
}

GameObject* GameObjectCache::Get(int id)
{
    DebugAssert(id >= 0, "ERROR: Tried to get nonexistent id.\n");
    if(id < 0)
        return nullptr;
    return &entities[ID_Index[id]];
}


void GameObjectCache::DestroyNow(int id)
{
    DebugAssert(id >= 0, "ERROR: Tried to delete nonexistent id.\n");
    deactivateGameObject(id);
}

    
void GameObjectCache::DestroyLater(int id)
{
    cleanupList.push_back(id);
}

void GameObjectCache::DestroyCleanup()
{
    cleanupList.sort();
    cleanupList.unique();
    for(std::list<int>::iterator iter = cleanupList.begin(); iter != cleanupList.end(); ++iter)
    {    
        deactivateGameObject(*iter);
    }
    cleanupList.clear();
}


void GameObjectCache::Clear()
{
    int size = entities.size();
    entities.clear();
    freeSlots.clear();
    ID_Index.clear();
    Reserve(DEFAULT_NUM_OBJECTS);
}

void GameObjectCache::Free()
{
    entities.clear();
    entities.swap(std::vector<GameObject>());
    freeSlots.clear();
    freeSlots.swap(std::list<int>());
}

int GameObjectCache::generateID()
{
    return genID++;
}

void GameObjectCache::deactivateGameObject(int id)
{
    GameObject& go = entities[ID_Index[id]];
    deactivateGameObject(go);
}

void GameObjectCache::deactivateGameObject(GameObject& go)
{
    //deactivate slot
    int tempID = go.id;
    go.id = INACTIVE_GAMEOBJECT_ID;
    go.active = false;
    
    // add slot to free list
    freeSlots.push_back(ID_Index[tempID]);

    //remove from ID index
    ID_Index.erase(tempID);

    go.clearComponents();
}