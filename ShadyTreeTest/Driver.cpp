#include <windows.h>
#include <crtdbg.h>
#include "ST_API_Def.h"
#include "Core\ShadyTreeEngine.h"
#include "Window\WindowSystem.h"
#include "Graphics\GraphicsSystem.h"
#include "GameLogic\GameStateSystem.h"
#include "Physics\PhysicsSystem.h"

ST_API HWND ghMainWnd;
ST_API HINSTANCE ghInstance;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
#ifdef _DEBUG_LEAK_REPORTING
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    
    //create engine
    ShadyTreeEngine engine;

    //setup systems
    engine.AttachSystem(new WindowSystem("ShadyTreeTest", 1280, 720));

    GraphicsSystem* gs = new GraphicsSystem();
    gs->ToggleDebugDraw();
    engine.AttachSystem(gs);

    PhysicsSystem* ps = new PhysicsSystem();
    engine.AttachSystem(ps);

    GameStateSystem* gl = new GameStateSystem();
    gl->SetScreenSize(1280, 720);
    engine.AttachSystem(gl);

    //Initialize
    engine.Initialize();
    gs->setClearColor(Color(0,0,0,1.0f));

    //Load
    Resources::Instance().parseResourceIDs("resources");
    engine.Load();
    ps->generateDebugDraw();

    //run
    engine.Run();

    //free resources
    engine.Free();

    return 0;
}

