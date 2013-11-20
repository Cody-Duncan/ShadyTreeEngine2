#pragma once
#include "AbstractComponentCache.h"
#include <vector>

#define INACTIVE_ID -1;

template<class T>
class ComponentCache :public AbstractComponentCache
{
public:
    ComponentCache(void);
    ~ComponentCache(void);

    T* Generate()
    {
        if(freeSlots.empty())
        {
            int index = storage.size();
            int id = generateID();
            storage.emplace_back(id, true);

            ID_Index[id] = index;

            return storage.back();
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
        return storage[ID_Index[id]];
    }

    void Delete(T& component)
    {
        int index = ID_Index[component.id];
        ID_Index[component.id] = INACTIVE_ID;
        freeSlots.push_back(index);

        component.active = false;
        component.id = INACTIVE_ID;
    }

    void Reserve(int size)
    {
        if(size > storage.size())
        {
            int lastIndex = storage.size();
            
            for(int i = lastIndex; i < storage.size(); ++i)
            {
                freeSlots.push_back(i);
                storage.emplace_back();
            }
        }
        
    }

private:
    std::vector<T> storage;
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

template<class T>
ComponentCache<T>::ComponentCache(void)
{
}

template<class T>
ComponentCache<T>::~ComponentCache(void)
{
}
