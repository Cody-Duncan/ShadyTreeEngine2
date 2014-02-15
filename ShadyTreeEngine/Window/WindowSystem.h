#pragma once
#include "Core\ISystem.h"


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
    virtual void Free();
    virtual void RecieveMessage(Message* msg);

private: 
    bool GenerateWindow(WNDPROC WndProc, int width, int height);
};

