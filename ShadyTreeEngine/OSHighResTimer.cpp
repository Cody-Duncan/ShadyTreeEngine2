#include "OSHighResTimer.h"

LARGE_INTEGER frequencyTemp;
__int64 readHiResTimerFrequency()
{
        #ifdef WIN32   // Windows system specific
                QueryPerformanceFrequency(&frequencyTemp);
                return frequencyTemp.QuadPart;
        #else          // Unix based system specific
        
        #endif
}

LARGE_INTEGER timerTemp;
__int64 readHiResTimer()
{
        #ifdef WIN32   // Windows system specific
                QueryPerformanceCounter(&timerTemp);
                return timerTemp.QuadPart;
        #else          // Unix based system specific
        
        #endif
}