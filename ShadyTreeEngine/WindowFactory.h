#pragma once

#include <windows.h>

extern HWND ghMainWnd;
extern HINSTANCE ghInstance;

extern bool GenerateWindow(HINSTANCE hInstance, int show, WNDPROC WndProc);


