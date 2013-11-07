#include "GraphicsTestUser.h"

#include "DirectX_GraphicsDevice.h"
#include "MathLib.h"
#include "Vertex.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <sstream>
#include <random>

GraphicsTestUser::GraphicsTestUser(GraphicsDevice* in) : gd(in)
{
}


GraphicsTestUser::~GraphicsTestUser(void)
{
}

int GraphicsTestUser::init()
{
    createShaders();
    createVertexBuffers();
    setWorldViewProj();
    setTestTexture();
    return 0;
}


int GraphicsTestUser::createShaders()
{
    gd->createVertexShader("Tutorial07.fx","VS", "vs_4_0", &v);
    gd->createPixelShader("Tutorial07.fx","PS", "ps_4_0", &p);

    return 0;
}


int GraphicsTestUser::createVertexBuffers()
{
    // Create vertex buffer
    using namespace DirectX;
    Vertex vertices[] =
    {
        { Vector4( -1.0f, 1.0f, -1.0f, 1.0f), Vector2( 1.0f, 0.0f ) },
        { Vector4( 1.0f, 1.0f, -1.0f, 1.0f), Vector2( 0.0f, 0.0f ) },
        { Vector4( 1.0f, 1.0f, 1.0f, 1.0f), Vector2( 0.0f, 1.0f ) },
        { Vector4( -1.0f, 1.0f, 1.0f, 1.0f), Vector2( 1.0f, 1.0f ) },

        { Vector4( -1.0f, -1.0f, -1.0f, 1.0f), Vector2( 0.0f, 0.0f ) },
        { Vector4( 1.0f, -1.0f, -1.0f, 1.0f), Vector2( 1.0f, 0.0f ) },
        { Vector4( 1.0f, -1.0f, 1.0f, 1.0f), Vector2( 1.0f, 1.0f ) },
        { Vector4( -1.0f, -1.0f, 1.0f, 1.0f), Vector2( 0.0f, 1.0f ) },

        { Vector4( -1.0f, -1.0f, 1.0f, 1.0f), Vector2( 0.0f, 1.0f ) },
        { Vector4( -1.0f, -1.0f, -1.0f, 1.0f), Vector2( 1.0f, 1.0f ) },
        { Vector4( -1.0f, 1.0f, -1.0f, 1.0f), Vector2( 1.0f, 0.0f ) },
        { Vector4( -1.0f, 1.0f, 1.0f, 1.0f), Vector2( 0.0f, 0.0f ) },

        { Vector4( 1.0f, -1.0f, 1.0f, 1.0f), Vector2( 1.0f, 1.0f ) },
        { Vector4( 1.0f, -1.0f, -1.0f, 1.0f), Vector2( 0.0f, 1.0f ) },
        { Vector4( 1.0f, 1.0f, -1.0f, 1.0f), Vector2( 0.0f, 0.0f ) },
        { Vector4( 1.0f, 1.0f, 1.0f, 1.0f), Vector2( 1.0f, 0.0f ) },

        { Vector4( -1.0f, -1.0f, -1.0f, 1.0f), Vector2( 0.0f, 1.0f ) },
        { Vector4( 1.0f, -1.0f, -1.0f, 1.0f), Vector2( 1.0f, 1.0f ) },
        { Vector4( 1.0f, 1.0f, -1.0f, 1.0f), Vector2( 1.0f, 0.0f ) },
        { Vector4( -1.0f, 1.0f, -1.0f, 1.0f), Vector2( 0.0f, 0.0f ) },

        { Vector4( -1.0f, -1.0f, 1.0f, 1.0f), Vector2( 1.0f, 1.0f ) },
        { Vector4( 1.0f, -1.0f, 1.0f, 1.0f), Vector2( 0.0f, 1.0f ) },
        { Vector4( 1.0f, 1.0f, 1.0f, 1.0f), Vector2( 0.0f, 0.0f ) },
        { Vector4( -1.0f, 1.0f, 1.0f, 1.0f), Vector2( 1.0f, 0.0f ) }
    };

    // Create index buffer
    unsigned int indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    Mesh* newMesh = gd->generateMesh("mesh1");
    newMesh->vertices = vertices;
    newMesh->vertexCount = ARRAYSIZE(vertices);
    newMesh->indices = indices;
    newMesh->indexCount = ARRAYSIZE(indices);

    int result = gd->createVertexIndexBuffer(newMesh, &bufH);
    m = newMesh->handle();
    
    if(result)
        return result;

    return 0;
}


int GraphicsTestUser::setWorldViewProj()
{
    gd->setProjection();
    //gd->setOrthographicProjection();
    
    return 0;
}

int GraphicsTestUser::setTestTexture()
{
    // Load the Texture
    
    gd->createTexture("seafloor2.png", &t);

    return 0;
}

float tf = 0.0f;
int count = 0;

void GraphicsTestUser::testdraw()
{
    count++;
    tf += 0.0004f;
    Vector4 eye( 0.0f, 0.0f, 5.0f, 0.0f);
    Vector4 at(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 up(0.0f, 1.0f, 0.0f, 0.0f );
    gd->setView( eye, at, up );
   
    gd->setClearColor(Color(0.4f,0.6f,0.9f,1.0f));
    gd->clearRenderTarget();

    gd->setVertexShader(v);
    gd->setPixelShader(p);

    gd->BeginDraw();

        Matrix id;
        id.Identity();
        Matrix rotmat = Matrix::CreateRotationY(tf) * Matrix::CreateRotationX(tf/2);
        Matrix scaleMat = Matrix::CreateScale(1.0f);
        Matrix a = scaleMat * rotmat;

            gd->setWorld(a);

            gd->Draw(m, t);
        
    gd->EndDraw();

    gd->SwapBuffer();
}