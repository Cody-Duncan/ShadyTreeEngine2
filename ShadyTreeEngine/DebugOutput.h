
#include <stdio.h> // for va_list et al
#include "ST_API_Def.h"

ST_API extern int VDebugPrintF(const char* format, va_list& argList);

ST_API extern int DebugPrintF(const char* format, ...);

ST_API extern int g_verbosity;
ST_API extern void VerboseDebugPrintF(int verbosity,const char* format, ...);

ST_API int InitDebugLogFile();
ST_API int CloseDebugLogFile();
ST_API int OutputDebugLog(const char* message);

#ifdef _DEBUG
#define DebugPrintf(format, ...) DebugPrintF(format, __VA_ARGS__)
#define DebugVerbosePrintf(verbosity,format, ...) VerboseDebugPrintF(verbosity,format, __VA_ARGS__)
#define DebugLogOpen() InitDebugLogFile()
#define DebugLogClose() CloseDebugLogFile()
#else
#define DebugPrintf(format, ...) 
#endif

#define CHECKHR(hr, message) \
if(FAILED(hr)){             \
    DebugPrintf(message);   \
    return hr;     }        