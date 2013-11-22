#include "stdafx.h"
#include "GameTimer.h"


GameTimer::GameTimer(void) : current(0), last(0), elapsedSeconds(0.0), deltaTime(0.0f), timeScale(1.0)
{
}


GameTimer::~GameTimer(void)
{
}

void GameTimer::Start()
{
    last = readHiResTimer();
}

void GameTimer::Update()
{
    current = readHiResTimer();
    double seconds = (current - last) / readHiResTimerFrequency() * timeScale;
    elapsedSeconds += seconds;
    deltaTime = (float) seconds * 1000.0f;
}

float GameTimer::GetDeltaTime()
{
    return deltaTime;
}

void GameTimer::setTimeMultiplier(double s)
{
    timeScale = s;
}

