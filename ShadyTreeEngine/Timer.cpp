#include "Timer.h"

Timer::Timer()
{

}

void Timer::Start()
{
    passed = 0.0f;
}

void Timer::Start(float wait)
{
    this->wait = wait;
    passed = 0.0f;
}

bool Timer::Tick(float deltaTime)
{
    passed += deltaTime;
    return IsDone();
}

bool Timer::IsDone()
{
    return passed > wait;
}

void Timer::hardEnd()
{
    passed = wait + 1;
}
