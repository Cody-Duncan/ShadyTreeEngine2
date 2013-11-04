#include "WindowFactory.h"
#include <tchar.h>

ST_API HWND ghMainWnd = 0;
ST_API HINSTANCE ghInstance = 0;


bool GenerateWindow(HINSTANCE hInstance, int show, WNDPROC WndProc)
{
    ghInstance = hInstance;

    static TCHAR szWindowClass[] = _T("shady_tree_app");
    static TCHAR szTitle[] = _T("ShadyTree Test");

    WNDCLASSEX  wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW; //redraw if resized or moved that changes height or width
    wcex.lpfnWndProc    = WndProc;                 //pass the WndProc function
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
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

    //Create Window - get an HWND
    ghMainWnd = CreateWindow(
        szWindowClass,          // Registered WNDCLASS instance to use.
        szTitle,                // window title
        WS_OVERLAPPEDWINDOW,    // style flags
        CW_USEDEFAULT,          // x-coordinate
        CW_USEDEFAULT,          // y-coordinate
        CW_USEDEFAULT,          // width
        CW_USEDEFAULT,          // height
        0,                      // parent window
        0,                      // menu handle
        hInstance,              // app instance
        0                       // extra creation parameters
    ); 

    if(ghMainWnd == 0)
    {
        MessageBox(0, L"CreateWindow FAILED", 0, 0);
        return false;
    }

    ShowWindow(ghMainWnd, show);
    UpdateWindow(ghMainWnd);
    return true;
}
