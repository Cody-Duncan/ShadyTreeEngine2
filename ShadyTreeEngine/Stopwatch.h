//////////////////////////////////////////////////////////////////////////
// FILE: Stopwatch.h
// DESC: Implementation of CStopwatch class, to measure C++ code
//       performances.
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include "OSHighResTimer.h"

class Stopwatch
{
public:
    
    Stopwatch()
        : m_startCount(0), m_elapsedTimeSec(0.0)
    {
        // Does some initialization to get consistent results for all tests:
        // Confine the test to run on a single processor,
        // to get consistent results for all tests.
        SetThreadAffinityMask(GetCurrentThread(), 1);
        SetThreadIdealProcessor(GetCurrentThread(), 0);
        Sleep(1);
    }

    // Starts measuring performance
    // (to be called before the block of code to measure).
    void Start()
    {
        m_elapsedTimeSec = 0.0;
        m_startCount = Counter();
    }

    // Stops measuring performance
    // (to be called after the block of code to measure).
    void Stop()
    {
        LONGLONG stopCount = Counter();
        // time is measured in seconds
        m_elapsedTimeSec = (stopCount - m_startCount) * 1.0 / Frequency();
        m_startCount = 0;
    }

    // Returns total elapsed time (in seconds) in Start-Stop interval.
    double ElapsedTimeSec() const
    {
        return m_elapsedTimeSec;
    }

    // Returns total elapsed time (in milliseconds) in Start-Stop interval.
    double ElapsedTimeMilliSec() const
    {
        return m_elapsedTimeSec * 1000.0;
    }

private:

    LONGLONG m_startCount;
    double m_elapsedTimeSec;

    // Returns current value of high-resolution performance counter.
    LONGLONG Counter() const
    {
        return readHiResTimer();
    }

    // Returns the frequency (in counts per second) of the
    // high-resolution performance counter.
    LONGLONG Frequency() const
    {
        return readHiResTimerFrequency();
    }

private:
    // *** Ban copy ***
    Stopwatch(const Stopwatch &);
    Stopwatch & operator=(const Stopwatch &);
};
