#pragma once

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

#ifndef HIGHRESTIMER
#define HIGHRESTIMER

ST_API __int64 readHiResTimerFrequency();
ST_API __int64 readHiResTimer();

#endif