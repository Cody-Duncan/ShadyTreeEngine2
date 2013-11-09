#include "stdafx.h"
#include "DebugTimer.h"


double SecondsPerTick = 0;
long long StartTime = 0;
long long EndTime = 0;

void InitDebugTimer()
{
    long long countsPerSec = readHiResTimerFrequency();
    SecondsPerTick = 1.0/(double)countsPerSec;
}

void DTimerStart()
{
    StartTime = readHiResTimer();
}

void DTimerEnd()
{
    EndTime = readHiResTimer();
}

double DTimeSecs()
{
    return ((EndTime - StartTime)*SecondsPerTick);
}