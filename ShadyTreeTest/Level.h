#pragma once

#include "BB.h"

class DeSerializer;

class Level
{
public:
    Level(void);
    ~Level(void);

    void Initialize(std::string resID, DeSerializer& s, int _Height, int _Width, float _BorderRatio);
    bool IsOnPlatform(Vector2 position, BB* body);
    bool IsOutsideLevel(Vector2 position);
    void Dispose();

    std::vector<GameObject*> levelStuff;

    BB_Rectangle LevelSpace;
    Vector2 LevelPos;

    Vector2 playerStart;
    std::vector<Vector2> enemyStartPositions;
};

