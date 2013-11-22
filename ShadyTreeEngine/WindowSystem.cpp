#include "stdafx.h"
#include "WindowSystem.h"
#include <tchar.h>
#include "WindowGlobals.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


WindowSystem::WindowSystem(const char* windowTitle, int width, int height)
{
    //create window
    if(!GenerateWindow(WndProc, width, height))
    {
        DebugPrintF("Failed to create Window");
        assert(false && "Failed to create Window");
    }

}

WindowSystem::~WindowSystem(void)
{
}

bool WindowSystem::GenerateWindow(WNDPROC WndProc, int width, int height)
{
    static TCHAR szWindowClass[] = _T("shady_tree_app");
    static TCHAR szTitle[] = _T("ShadyTree Test");

    HINSTANCE inst = GetModuleHandle(NULL);

    RECT winSize = {0, 0, width, height};
    AdjustWindowRect(&winSize,			
                    CS_HREDRAW | CS_VREDRAW,	
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
            //MessageQuit q;
            //CORE->BroadcastMessage(&q);
        }
    }
}

void WindowSystem::Init() { ActivateWindow(); }
void WindowSystem::Load() {}
void WindowSystem::Unload() {};

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

