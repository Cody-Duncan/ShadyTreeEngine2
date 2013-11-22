#pragma once

#include "OSHighResTimer.h"

class GameTimer
{
public:
    GameTimer(void);
    ~GameTimer(void);

    long long current;
    long long last;
    double elapsedSeconds;
    float deltaTime;
    double timeScale;

    void Start();
    void Update();
    float GetDeltaTime();
    void setTimeMultiplier(double s);
};

