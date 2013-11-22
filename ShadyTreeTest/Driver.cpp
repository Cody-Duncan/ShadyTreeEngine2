#include <windows.h>
#include <crtdbg.h>
#include "ST_API_Def.h"
#include "ShadyTreeEngine.h"


ST_API HWND ghMainWnd;
ST_API HINSTANCE ghInstance;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    
    ShadyTreeEngine engine;
    engine.Initialize(hInstance, nShowCmd);
    engine.Run();


    return 0;
}

