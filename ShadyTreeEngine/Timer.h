#pragma once

class Timer
{
public:
    ST_API Timer();
    ST_API void Start();
    ST_API void Start(float wait);
    ST_API bool Tick(float deltaTime);
    ST_API void hardEnd();
    ST_API bool IsDone();

    float wait;
    float passed;
};