#include "stdafx.h"
#include "WindowSystem.h"
#include <tchar.h>
#include "WindowGlobals.h"
#include "ShadyTreeEngine.h"
#include "InputState.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


WindowSystem::WindowSystem(const char* windowTitle, int width, int height)
{
    //create window
    DebugPrintf("WINDOW: Creating Window\n");
    if(!GenerateWindow(WndProc, width, height))
    {
        DebugPrintf("Failed to create Window");
        assert(false && "Failed to create Window");
    }

}

WindowSystem::~WindowSystem(void)
{
    if(gINPUTSTATE)
        delete gINPUTSTATE;
}

bool WindowSystem::GenerateWindow(WNDPROC WndProc, int width, int height)
{
    static TCHAR szWindowClass[] = _T("shady_tree_app");
    static TCHAR szTitle[] = _T("ShadyTree Test");

    HINSTANCE inst = GetModuleHandle(NULL);

    RECT winSize = {0, 0, width, height};
    AdjustWindowRect(&winSize,			
                    WS_OVERLAPPEDWINDOW,
                    FALSE);	

    WNDCLASSEX  wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW; //redraw if resized or moved that changes height or width
    wcex.lpfnWndProc    = WndProc;                 //pass the WndProc function
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = inst;
    wcex.hIcon          = LoadIcon(inst, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if(!RegisterClassEx(&wcex))
    {
        MessageBox(0, L"RegisterClass FAILED", 0, 0);
        return false;
    }

    ghInstance = inst;

    //Create Window - get an HWND
    ghMainWnd = CreateWindow(
        szWindowClass,          // Registered WNDCLASS instance to use.
        szTitle,                // window title
        WS_OVERLAPPEDWINDOW,    // style flags
        CW_USEDEFAULT,          // x-coordinate
        CW_USEDEFAULT,          // y-coordinate
        winSize.right - winSize.left,  // width
        winSize.bottom - winSize.top,   // height
        0,                      // parent window
        0,                      // menu handle
        inst,                   // app instance
        0                       // extra creation parameters
    ); 

    if(ghMainWnd == 0)
    {
        MessageBox(0, L"CreateWindow FAILED", 0, 0);
        return false;
    }

    return true;
}

void WindowSystem::ActivateWindow()	//Activate the game window so it is actually visible
{
    ShowWindow(ghMainWnd, SW_SHOWDEFAULT);
    UpdateWindow(ghMainWnd);
}

void WindowSystem::Update(float deltaTime)
{
    MSG msg = {0};
    
    while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );

        if (msg.message == WM_QUIT)
        {
            Message quitmsg(MessageType::Quit);
            CORE->BroadcastMessage( &quitmsg );
        }
    }
}

void WindowSystem::Init() 
{ 
    ActivateWindow(); 
    gINPUTSTATE = new InputState();
    gINPUTSTATE->Init();
}
void WindowSystem::Load() {}
void WindowSystem::Unload() {};
void WindowSystem::Free()
{
    DebugPrintf("WINDOW: Deallocating Window\n");
    delete gINPUTSTATE;
    gINPUTSTATE = 0;
}
void WindowSystem::RecieveMessage(Message* msg)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   
    switch(msg)
    {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            gINPUTSTATE->updateKey(msg, wParam, lParam);
            break;

        case WM_KEYDOWN:
        case WM_KEYUP:
            gINPUTSTATE->updateKey(msg, wParam, lParam);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

