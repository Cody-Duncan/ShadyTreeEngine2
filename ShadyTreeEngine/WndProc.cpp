#include "stdafx.h"
#include "WndProc.h"
#include "WindowFactory.h"


LRESULT CALLBACK ShadyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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