#pragma once

class DeSerializer;

class Level
{
public:
    Level(void);
    ~Level(void);

    void Initialize(std::string resID, DeSerializer& s);

    std::vector<GameObject*> levelStuff;
};

