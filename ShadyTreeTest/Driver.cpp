#include <windows.h>
#include <crtdbg.h>
#include "ST_API_Def.h"
#include "ShadyTreeEngine.h"
#include "WindowSystem.h"
#include "GraphicsSystem.h"


ST_API HWND ghMainWnd;
ST_API HINSTANCE ghInstance;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    
    ShadyTreeEngine engine;
    engine.AttachSystem(new WindowSystem("ShadyTreeTest", 1280, 720));
    engine.AttachSystem(new GraphicsSystem() );
    engine.Initialize();
    engine.Run();


    return 0;
}

