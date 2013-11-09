#include "SpriteBatchTestUser.h"


SpriteBatchTestUser::~SpriteBatchTestUser(void)
{
}

int SpriteBatchTestUser::init()
{
    sb = generateSpriteBatch(gd);
    sb->Init();
    setTestTexture();

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
    for(int i = 0; i < 249; i++)
    {
        Matrix m = Matrix::CreateTranslation(i*2, i*2, 0);
        Rectangle2& r = textures[i%4];
        sb->Draw(t,m, r);
    }

    counter++;
    OutputDebugStringA("Mark\n");

    sb->End();
}
