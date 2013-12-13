#pragma once

class DeSerializer;
class BB;

class Level
{
public:
    Level(void);
    ~Level(void);

    void Initialize(std::string resID, DeSerializer& s);
    bool IsOnPlatform(Vector2 position, BB* body);

    std::vector<GameObject*> levelStuff;
};

