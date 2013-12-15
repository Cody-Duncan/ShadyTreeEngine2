#include "StdAfx2.h"
#include "IntroScreen.h"
#include "PositionalComponent.h"
#include "GraphicsComponent.h"
 #include <string>

#include "InputState.h"
#include "ShadyTreeEngine.h"
#include "ChangeStateMessage.h"

IntroScreen::IntroScreen(void) : currentSelection(SelectionState::NewGame),
    screenWidth(0), screenHeight(0),
    introScreenObj(nullptr), selector(nullptr)
{
}


IntroScreen::~IntroScreen(void)
{
}

void IntroScreen::Init()
{
    
}

void IntroScreen::Load()
{
    DebugAssert(screenWidth > 0, "Screen width is 0");
    DebugAssert(screenHeight > 0, "Screen height is 0");

    IResources& res = Resources::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    GameObjectFactory& GOF = GameObjectFactory::Instance();

    res.LoadTextureRes("IntroScreen");
    res.LoadTextureRes("selector");

    introScreenObj = GOF.createGraphicalEntity("IntroScreen", Vector2((float)screenWidth/2, (float)screenHeight/2) );
    PositionalComponent& introPos = *introScreenObj->getComponent<PositionalComponent>();
    GraphicsComponent& introGraphics = *introScreenObj->getComponent<GraphicsComponent>();

    //read the menu selection locations from file.
    std::iostream* selectorPosFile = res.GetFileData(res.LoadDataRes("menuSelectionPositions"));
    while(!selectorPosFile->eof())
    {
        std::string line;
        std::getline(*selectorPosFile, line);
        int comma = line.find(',');

        int x = std::stoi(line.substr(0,comma));
        int y = std::stoi(line.substr(comma+1));
        menuSelectionPositions.emplace_back((float)x,(float)y);
    }

    //set the menu selector the location matching current selection.
    selector = GOF.createGraphicalEntity("selector", Vector2(0,0));
    PositionalComponent& selectorPos = *selector->getComponent<PositionalComponent>();

    selectorPos.position = introPos.position + menuSelectionPositions[currentSelection];

}

void IntroScreen::Update(float deltaTime)
{
    if(gINPUTSTATE->keyPressed(VK_UP) || gINPUTSTATE->keyPressed(VK_DOWN))
    {
        ((int&)currentSelection) = (currentSelection+1)%SelectionState_Num;
    }

    PositionalComponent& introPos = *introScreenObj->getComponent<PositionalComponent>();
    PositionalComponent& selectorPos = *selector->getComponent<PositionalComponent>();
    selectorPos.position = introPos.position + menuSelectionPositions[currentSelection];

    if(gINPUTSTATE->keyPressed(VK_RETURN))
    {
        switch(currentSelection)
        {
        case SelectionState::NewGame:
            CORE->BroadcastMessage(&ChangeStateMessage(State::GameState));
            break;
        case SelectionState::EndGame:
            CORE->BroadcastMessage(&Message(MessageType::Quit));
            break;
        }
        
    }
}

void IntroScreen::Unload()
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    GOC.DestroyNow(introScreenObj->id);
    GOC.DestroyNow(selector->id);
    introScreenObj = nullptr;
    selector = nullptr;
}

void IntroScreen::Free()
{
    
}

void IntroScreen::RecieveMessage(Message* msg)
{
    
}

void IntroScreen::SetScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}