#pragma once

class GameObjectFactory
{
public:
    static GameObjectFactory& Instance()
    {
        static GameObjectFactory factory;
        return factory;
    }

    GameObject* createGraphicalEntity(std::string textureResID, Vector2 centerPos);
    void addArchetype(std::string archetypeName, int id);
    GameObject* cloneArchetype(std::string archetypeName);

    void addEnemyType(std::string archetypeName);
    void addPowerupType(std::string powerupTypeName);
    
    std::vector<std::string> enemyTypes;
    std::vector<std::string> powerupTypes;

    void clearArchetypes();
    void clearSubtypes();

private:
    GameObjectFactory(void);
    ~GameObjectFactory(void);
    GameObjectFactory( GameObjectFactory& rhs);
    GameObjectFactory& operator=(GameObjectFactory& rhs);

    std::unordered_map<std::string, int> archetypes;
    

    bool hasArchetype(std::string name);
};

