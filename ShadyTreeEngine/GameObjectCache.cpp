#include "stdafx.h"
#include "GameObjectCache.h"

int GameObjectCache::genID = 0;


#define DEFAULT_NUM_OBJECTS 200
#define INACTIVE_GAMEOBJECT_ID -1;


GameObjectCache& GameObjectCache::Instance()
{
    static GameObjectCache factory;
    return factory;
}

GameObjectCache::GameObjectCache(void)
{
    entities.resize(DEFAULT_NUM_OBJECTS);
    for(int i = 0; i < DEFAULT_NUM_OBJECTS; ++i)
    {
        freeSlots.push_back(i);
    }
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
    }
}


GameObject* GameObjectCache::Create()
{
    if(freeSlots.empty()) //use a free slot if available
    {
        int index = entities.size();
        int id = generateID();
        entities.emplace_back();

        ID_Index[id] = index;

        return &entities.back();
    }
    else //make a new slot, use it
    {
        int index = freeSlots.front();
        int id = generateID();
        freeSlots.pop_front();
            
        GameObject& go = entities[index];
        go.id = id;
        go.active = true;

        ID_Index[id] = index;

        return &entities[index];
    }
}

GameObject* GameObjectCache::Get(int id)
{
    return &entities[ID_Index[id]];
}


void GameObjectCache::DestroyNow(int id)
{
    deactivateGameObject(id);
}

    
void GameObjectCache::DestroyLater(int id)
{
    cleanupList.push_back(id);
}

void GameObjectCache::DestroyCleanup()
{
    for(std::list<int>::iterator iter = cleanupList.begin(); iter != cleanupList.end(); ++iter)
    {
        deactivateGameObject(*iter);
    }
}


void GameObjectCache::Clear()
{
    int size = entities.size();
    entities.clear();
    freeSlots.clear();
    for(int i = 0; i < size; ++i)
    {
        freeSlots.push_back(i);
    }
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
    go.id = INACTIVE_GAMEOBJECT_ID;
    go.active = false;
    
    // add slot to free list
    freeSlots.push_back(ID_Index[go.id]);

    //remove from ID index
    ID_Index.erase(go.id);

    go.clearComponents();
}