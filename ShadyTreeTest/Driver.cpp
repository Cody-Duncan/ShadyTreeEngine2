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
#include "DebugTimer.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ST_API HWND ghMainWnd;
ST_API HINSTANCE ghInstance;

GraphicsDevice* device;
//GraphicsTestUser* puser;
SpriteBatchTestUser* sbUser;

void update();
void draw();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    DebugLogOpen();
    InitDebugTimer();
    
    //_CrtSetBreakAlloc(172);

    DTimerStart();

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

    SpriteBatchTestUser sbUserNew(device);
    sbUserNew.init();
    sbUser = &sbUserNew;

    DTimerEnd();
    DebugPrintf("Timer for init: %f", DTimeSecs());

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