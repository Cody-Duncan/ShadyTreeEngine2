#pragma once

#include <windows.h>


extern ST_API HWND ghMainWnd;
extern ST_API HINSTANCE ghInstance;

ST_API extern bool GenerateWindow(HINSTANCE hInstance, int show, WNDPROC WndProc);


