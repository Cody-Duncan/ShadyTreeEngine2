#pragma once
#include "ISystem.h"


class WindowSystem : public ISystem
{
public:
    ST_API WindowSystem(const char* windowTitle, int ClientWidth, int ClientHeight);
    ~WindowSystem(void);

    void ActivateWindow();	//Activate the game window so it is actually visible

    virtual void Init();
    virtual void Load();
    virtual void Update(float deltaTime);
    virtual void Unload();

private: 
    bool GenerateWindow(WNDPROC WndProc, int width, int height);
};

