#pragma once
#include <vector>
#include <unordered_map>
#include "GameObject\Component.h"

#define INACTIVE_ID -1

class AbstractComponentCache
{
public:
     virtual Component* Generate() = 0;
    virtual void Reserve(unsigned int size) = 0;
    virtual void Delete(int id) = 0;
    virtual void Clear() = 0;
    virtual void Free() = 0;
    virtual Component* GetComponent(int i) = 0;
};


template<class T>
class ComponentCache :public AbstractComponentCache
{
public:
    ComponentCache(void)
    {}
    ~ComponentCache(void)
    {}

    T* Generate()
    {
        if(freeSlots.empty())
        {
            int index = storage.size();
            int id = generateID();
            storage.emplace_back(id, true);

            ID_Index[id] = index;

            return &storage.back();
        }
        else
        {
            int index = freeSlots.front();
            int id = generateID();
            freeSlots.pop_front();
            
            T& component = storage[index];
            component.id = id;
            component.active = true;

            ID_Index[id] = index;

            return &storage[index];
        }
    }

    T* Retrieve(int id)
    {
        return &storage[ID_Index[id]];
    }

    void Delete(int id)
    {
        int index = ID_Index[id];
        T& component = storage[index];
        ID_Index.erase(id);//[id] = INACTIVE_ID;
        freeSlots.push_back(index);

        component.active = false;
        component.id = INACTIVE_ID;
        component.parentID = INACTIVE_ID;
    }

    void Reserve(unsigned int size)
    {
        if(size > storage.size())
        {
            unsigned int lastIndex = storage.size();
            
            for(unsigned int i = lastIndex; i < size; ++i)
            {
                freeSlots.push_back(i);
                storage.emplace_back(INACTIVE_ID, false);
            }
        }
    }

    void Clear()
    {
        storage.clear();
        ID_Index.clear();
        freeSlots.clear();
    }

    void Free()
    {
        Clear();
        //storage.swap(std::vector<T>());
        ID_Index.swap(std::unordered_map<int, int>());
        freeSlots.swap(std::list<int>());
    }

    virtual Component* GetComponent(int id)
    {
        return &storage[ID_Index[id]];
    }

    std::vector<T> storage;

private:
    std::list<int> freeSlots;
    std::unordered_map<int, int> ID_Index;

    static int genID;
    static int generateID()
    {
        return genID++;
    }
};

template<class T>
int ComponentCache<T>::genID = 0;

