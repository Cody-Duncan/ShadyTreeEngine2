#pragma once

class GameObjectFactory
{
public:
    static GameObjectFactory& Instance()
    {
        static GameObjectFactory factory;
        return factory;
    }

    GameObject* createGraphicalEntity();
    void addArchetype(std::string archetypeName, int id);
    GameObject* cloneArchetype(std::string archetypeName);

private:
    GameObjectFactory(void);
    ~GameObjectFactory(void);
    GameObjectFactory( GameObjectFactory& rhs);
    GameObjectFactory& operator=(GameObjectFactory& rhs);

    std::unordered_map<std::string, int> archetypes;

    bool hasArchetype(std::string name);
};

