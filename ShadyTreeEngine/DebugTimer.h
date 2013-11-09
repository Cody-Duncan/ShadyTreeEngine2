#pragma once

#include "OSHighResTimer.h"

extern double SecondsPerTick;

ST_API void InitDebugTimer();
ST_API void DTimerStart();
ST_API void DTimerEnd();
ST_API double DTimeSecs();
