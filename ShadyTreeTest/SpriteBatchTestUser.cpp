#include "SpriteBatchTestUser.h"
#include "DebugOutput.h"
#include "DebugTimer.h"

SpriteBatchTestUser::~SpriteBatchTestUser(void)
{
    sb->Dispose();
    delete[] matrixArray;
}

int SpriteBatchTestUser::init()
{
    sb = generateSpriteBatch(gd);
    sb->Init();
    setTestTexture();

    matrixArrayLen = 249;
    matrixArray = new Matrix[matrixArrayLen];
    for(int i = 0; i < matrixArrayLen; i++)
    {
        matrixArray[i] = Matrix::CreateTranslation(i*2.0f, i*2.0f, 0);
    }

    return 0;
}


int SpriteBatchTestUser::setTestTexture()
{
    sb->createTexture("textMultTexture.png", &t);
    return 0;
}

int counter = 0;
void SpriteBatchTestUser::testdraw()
{
    
    Matrix m2 = Matrix::CreateScale(10) * Matrix::CreateTranslation(20, 20, 0);
    Matrix m3 = Matrix::CreateScale(10) * Matrix::CreateTranslation(30, 30, 0);
    Matrix m4 = Matrix::CreateScale(10) * Matrix::CreateTranslation(40, 40, 0);

    static Rectangle2 textures[] = {
        Rectangle2(0, 0, 5, 5), //white
        Rectangle2(5, 0, 5, 5), //green
        Rectangle2(0, 5, 5, 5), //blue
        Rectangle2(5, 5, 5, 5) //red
    };

    sb->Begin();
    for(int i = 0; i < matrixArrayLen; i++)
    {
        Rectangle2& r = textures[i%4];
        
        sb->Draw(t, matrixArray[i], r);
        
    }

    counter++;

    //DTimerStart();
    sb->End();
    //DTimerEnd();
    //DebugPrintf("Draw Time in seconds: %f\n", DTimeSecs());

    
    
}
