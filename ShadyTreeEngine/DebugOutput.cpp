#include "DebugOutput.h"
#include <ctime>
#include <iostream>
#include <fstream>

int g_verbosity = 0;
std::ofstream debugLogFile;

int VDebugPrintF(const char* format, va_list& argList)
{
    const unsigned int MAX_CHARS = 1023;
    static char s_buffer[MAX_CHARS + 1];
    int charsWritten = vsnprintf_s(s_buffer, MAX_CHARS, format,  argList);
    s_buffer[MAX_CHARS] = '\0'; 

    OutputDebugLog(s_buffer);
    OutputDebugStringA(s_buffer);

    return charsWritten;
}

int DebugPrintF(const char* format, ...)
{
    va_list argList;
    va_start(argList, format);
    int charsWritten = VDebugPrintF(format, argList);
    va_end(argList);
    return charsWritten;
}

void VerboseDebugPrintF(int verbosity,const char* format, ...)
{
    if (g_verbosity >= verbosity)
    {
        va_list argList;
        va_start(argList, format);
        VDebugPrintF(format, argList);
        va_end(argList);
    }
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%X %A %B %d %Y", &tstruct);

    return buf;
}

int InitDebugLogFile() 
{
    debugLogFile.open ("errorLog.log", std::fstream::in | std::fstream::out | std::fstream::app);

    DebugPrintF("------------Starting Engine------------\nTimestamp: %s\n", currentDateTime().c_str());

    return 0;
}

int CloseDebugLogFile() 
{
    debugLogFile.close();
    return 0;
}

int OutputDebugLog(const char* message)
{
    if(debugLogFile.is_open())
    {
        debugLogFile << message;
        debugLogFile.flush();  
    }
    return 0;
}
