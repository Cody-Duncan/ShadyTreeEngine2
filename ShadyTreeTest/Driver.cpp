#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "MathLib.h"

#include "GraphicsDevice.h"
#include "GraphicsFactory.h"

#include "GraphicsTestUser.h"
#include "WindowFactory.h"
#include "ShadyTree_DLLAPI.h"
#include "DebugOutput.h"
#include "SpriteBatchTestUser.h"
#include "Stopwatch.h"
#include "ComponentFactory.h"
#include "Component.h"
#include "GameObject.h"
#include "GameObjectCache.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ST_API HWND ghMainWnd;
ST_API HINSTANCE ghInstance;

GraphicsDevice* device;
//GraphicsTestUser* puser;
SpriteBatchTestUser* sbUser;

void update();
void draw();

class Banana : public Component 
{
public: 
    Banana() {}
    Banana(int _id,  bool _active)
        : Component(_id,_active) {}
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    DebugLogOpen();

    Stopwatch watch;
    
    ComponentFactory& cf = ComponentFactory::Instance();
    cf.createComponentCache<Component>();
    Component* comp = cf.createComponent<Component>();
    Banana* comp3 = cf.createComponent<Banana>();
    Component* comp2 = cf.getComponent<Component>(comp->id);
    cf.deleteComponent<Component>(comp->id);
    bool result = cf.hasComponentCache<Component>();
    

    GameObject ent;
    ent.attachComponent(comp3);
    Banana* comp4 = ent.getComponent<Banana>();
    result = ent.hasComponent<Banana>();
    ent.removeComponent<Banana>();
    result = ent.hasComponent<Banana>();

    GameObjectCache& GoC = GameObjectCache::Instance();
    GameObject* ent = GoC.Create();
    
    //_CrtSetBreakAlloc(172);

    watch.Start();

    if(!GenerateWindow(hInstance, nShowCmd, WndProc))
        return 0;

    device = generateGraphicsDevice(DeviceAPI::DirectX11);
    if( device->Init() )
    {
        device->Free();
        delete device;
        return 1;
    }

    //GraphicsTestUser user(device);
    //user.init();
    //puser = &user;

    sbUser = new SpriteBatchTestUser(device);
    sbUser->init();


    watch.Stop();
    DebugPrintf("\nTimer for init: %f ms\n\n", watch.ElapsedTimeMilliSec());

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            update();
            draw();
        }

        Sleep(1);
    }


    delete sbUser;

    device->Free();
    delete device;

    return ( int )msg.wParam;

    DebugLogClose();

    return 0;
}

void update()
{

}
void draw()
{
    sbUser->testdraw();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch(msg)
    {
        case WM_LBUTTONDOWN:
            MessageBox(0, L"Hello, World", L"Hello", MB_OK);
            break;

        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE)
            DestroyWindow(ghMainWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}