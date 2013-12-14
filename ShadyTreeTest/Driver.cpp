#include <windows.h>
#include <crtdbg.h>
#include "ST_API_Def.h"
#include "ShadyTreeEngine.h"
#include "WindowSystem.h"
#include "GraphicsSystem.h"
#include "GameLogic.h"
#include "PhysicsSystem.h"

ST_API HWND ghMainWnd;
ST_API HINSTANCE ghInstance;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
#ifdef _DEBUG_LEAK_REPORTING
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    
    ShadyTreeEngine engine;

    engine.AttachSystem(new WindowSystem("ShadyTreeTest", 1280, 720));

    GraphicsSystem* gs = new GraphicsSystem();
    gs->ToggleDebugDraw();
    engine.AttachSystem(gs);

    PhysicsSystem* ps = new PhysicsSystem();
    engine.AttachSystem(ps);

    GameLogic* gl = new GameLogic();
    gl->SetPhysics(ps);
    gl->SetWorldDimension(720, 1270);
    engine.AttachSystem(gl);

    engine.Initialize();
    gs->setClearColor(Color(0,0,0,1.0f));
    engine.Load();
    ps->generateDebugDraw();
    engine.Run();
    engine.Free();


    return 0;
}

