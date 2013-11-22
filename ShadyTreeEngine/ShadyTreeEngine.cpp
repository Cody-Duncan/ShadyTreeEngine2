#include "ShadyTreeEngine.h"

#include "WindowFactory.h"
#include "GraphicsFactory.h"
#include "WndProc.h"

ShadyTreeEngine::ShadyTreeEngine()
{

}

ShadyTreeEngine::~ShadyTreeEngine()
{

}

void ShadyTreeEngine::Initialize(HINSTANCE hInstance, int show)
{
    DebugLogOpen();

    //create window
    if(!GenerateWindow(hInstance, show, ShadyWndProc))
    {
        DebugPrintF("Failed to create Window");
        assert(false && "Failed to create Window");
    }

    graphicsSys = new GraphicsSystem(); 
    graphicsSys->Init();
    
}

void ShadyTreeEngine::Run()
{
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
            Update();
            Draw();
        }

        Sleep(1);
    }
}

void ShadyTreeEngine::Free()
{
    graphicsSys->Unload();
    DebugLogClose();
}

void ShadyTreeEngine::Update()
{
    

}


void ShadyTreeEngine::Draw()
{
    graphicsSys->Update();
}