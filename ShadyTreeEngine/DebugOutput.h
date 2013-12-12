
#include <stdio.h> // for va_list et al
#include "ST_API_Def.h"

ST_API extern int VDebugPrintF(const char* format, va_list& argList);

ST_API extern int DebugPrintF(const char* format, ...);

ST_API extern int g_verbosity;
ST_API extern void VerboseDebugPrintF(int verbosity,const char* format, ...);

ST_API int InitDebugLogFile();
ST_API int CloseDebugLogFile();
ST_API int OutputDebugLog(const char* message);

ST_API void AssertPrintF(const char* format, char* file, int line, ...);
ST_API void AssertPrintF(const char* format, char* file, int line, ...);

#ifdef _DEBUG
#define DebugPrintf(format, ...) DebugPrintF(format, __VA_ARGS__)
#define DebugVerbosePrintf(verbosity,format, ...) VerboseDebugPrintF(verbosity,format, __VA_ARGS__)
#define DebugLogOpen() InitDebugLogFile()
#define DebugLogClose() CloseDebugLogFile()
#define DebugAssert(condition, format, ...) ((void)( (!!(condition)) || (AssertPrintF(format, __FILE__, __LINE__, __VA_ARGS__), 0) ), assert(condition))
#else
#define DebugPrintf(format, ...) 
#define DebugVerbosePrintf(verbosity,format, ...)
#define DebugLogOpen() 
#define DebugLogClose() 
#define DebugAssert(condition, format, ...) 
#endif

#define CHECKHR(hr, message) \
if(FAILED(hr)){             \
    DebugPrintf(message);   \
    return hr;     }        