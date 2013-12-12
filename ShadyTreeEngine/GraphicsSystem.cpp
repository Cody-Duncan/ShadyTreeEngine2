#include "GraphicsSystem.h"

#include "GraphicsDevice.h"
#include "SpriteBatch.h"
#include "PrimitiveBatch.h"

#include "GraphicsFactory.h"
#include "ComponentFactory.h"
#include "GraphicsComponent.h"
#include "PrimitiveGraphicsComponent.h"
#include "PositionalComponent.h"
#include "GameObject.h"
#include "GameObjectCache.h"
#include "DebugDrawComponent.h"

bool GraphicsSystem::IsDebugDrawOn()
{
    return debugDraw;
}

bool GraphicsSystem::ToggleDebugDraw()
{
    return debugDraw = !debugDraw;
}

void GraphicsSystem::Init()
{
    //create the graphics device TODO, replace with graphics system
    device = generateGraphicsDevice(DeviceAPI::DirectX11);
    if( device->Init() )
    {
        device->Free();
        delete device;
        device = 0;
        DebugPrintF("Failed to create Graphics Device");
        DebugAssert(false, "Failed to create Graphics Device");
    }

    spriteBatch = generateSpriteBatch(device);
    spriteBatch->Init();

    primitiveBatch = generatePrimitiveBatch(device);
    primitiveBatch->Init();
}

void GraphicsSystem::Load()
{
    
}

int count = 0;
double totalTime = 1.0f;
std::stringstream ss;
std::string resetString = "";
void GraphicsSystem::Update(float deltaTime)
{
    DebugAssert(spriteBatch, "No spritebatch initialized. Did you forget to call Init()?");

    ComponentFactory& CF = ComponentFactory::Instance();
    if(!CF.hasComponentCache<GraphicsComponent>() || !CF.hasComponentCache<PositionalComponent>() ) //check for any graphicsComponents
        return;

    //grab the caches
    std::vector<GraphicsComponent>& graphC = CF.getCache<GraphicsComponent>()->storage;  //graphics components
    std::vector<PrimitiveGraphicsComponent>& primC = CF.getCache<PrimitiveGraphicsComponent>()->storage;  //graphics components
    GameObjectCache& GOC = GameObjectCache::Instance();                                  //game objects

    //check for cache, set length to 0 to prevent drawing nothing.
    unsigned int graphicsCompLength = CF.hasComponentCache<GraphicsComponent>() ? graphC.size() : 0;
    unsigned int primitivCompLength = CF.hasComponentCache<PrimitiveGraphicsComponent>() ? primC.size() : 0;

    //BEGIN DRAWING
    device->clearRenderTarget();

    
    primitiveBatch->Begin();
    for(unsigned int i = 0; i < primitivCompLength; i++)
    {
        PrimitiveGraphicsComponent& pg= primC[i];
        if(pg.active)
        {
            GameObject& go = *GOC.Get(pg.parentID);
            PositionalComponent* posC = go.getComponent<PositionalComponent>();
            DebugAssert(posC, "Positional Component not found or null.");
            DebugAssert(posC->active, "Trying to draw active graphicsComponent, with nonactive positionalComponent.");

            //center the transform?
            Matrix transform = posC->rotationCentered ?
                Matrix::CreateTranslation(-pg.center.x/2, -pg.center.y/2, 0) * posC->Transform() :
                posC->Transform();

            primitiveBatch->DrawTriangles(pg.layer,pg.triangleListPoints.data(), pg.triangleListPoints.size(), transform, pg.color);
        }
    }
    primitiveBatch->End();
    
    

    spriteBatch->Begin(true);
    
        //draw all graphicsComponents
        for(unsigned int i = 0; i < graphicsCompLength; i++)
        {
            GraphicsComponent& g = graphC[i];
            if(g.active)
            {
                GameObject& go = *GOC.Get(g.parentID);
                PositionalComponent* posC = go.getComponent<PositionalComponent>();
                DebugAssert(posC, "Positional Component not found or null.");
                DebugAssert(posC->active, "Trying to draw active graphicsComponent, with nonactive positionalComponent.");

                //center the transform?
                Matrix transform = posC->rotationCentered ?
                    Matrix::CreateTranslation(-g.textureArea.dimensions.x/2, -g.textureArea.dimensions.y/2, 0) * posC->Transform() :
                    posC->Transform();

                spriteBatch->Draw(g.texture, transform, g.textureArea);
            }
        }

        //calculate and draw FPS
        ++count;
        totalTime += deltaTime;
        char buf[100];
        sprintf_s(buf, "FPS: %8.2f", (count / totalTime));
        spriteBatch->TextDraw(Vector2(1,1), buf);
        ss.str(resetString);

        if(totalTime > 1.0f)
        {
            count = 0;
            totalTime = 0;
        }
    
    //END SPRITE DRAWING
    spriteBatch->End();
    
    //DEBUG DRAWING
    if(debugDraw && CF.hasComponentCache<DebugDrawComponent>())
    {
        std::vector<DebugDrawComponent>& debugCache = CF.getCache<DebugDrawComponent>()->storage;  //graphics components
        primitiveBatch->Begin(true);
        for(unsigned int i = 0; i < debugCache.size(); i++)
        {
            DebugDrawComponent& pg= debugCache[i];
            if(pg.active)
            {
                GameObject& go            = *pg.parent();
                PositionalComponent* posC = go.getComponent<PositionalComponent>();
                GraphicsComponent* g      = go.getComponent<GraphicsComponent>();
                DebugAssert(posC, "Positional Component not found or null.");
                DebugAssert(posC->active, "Trying to draw active graphicsComponent, with nonactive positionalComponent.");

                //center the transform?
                Matrix transform = posC->rotationCentered ?
                    Matrix::CreateTranslation(-g->textureArea.dimensions.x/2, -g->textureArea.dimensions.y/2, 0) * posC->Transform() :
                    posC->Transform();

                primitiveBatch->DrawTriangles(10, pg.geometry.data(), pg.geometry.size(), transform, pg.color);
                primitiveBatch->DrawLines(11, pg.lines.data(), pg.lines.size(), transform, pg.color);
            }
        }
        primitiveBatch->End();
    }//END DEBUG DRAWING

    
    device->SwapBuffer();
}

void GraphicsSystem::Unload()
{
    
}

void GraphicsSystem::Free()
{
    DebugPrintf("GRAPHICS: Deallocated resources in Graphics Device\n");
    spriteBatch->Dispose();
    device->Free();

    delete spriteBatch;
    delete device;
}

void GraphicsSystem::RecieveMessage(Message* msg)
{
    
}

/// <summary>
/// Sets the color to clear the frame to.
/// </summary>
/// <remarks> Call after Initialize()</remarks>
/// <param name="color">The color.</param>
void GraphicsSystem::setClearColor(Color color)
{
    if(device)
        device->setClearColor(color);
}
