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


void SpriteBatchTestUser::testdraw()
{
    Matrix m1 = Matrix::CreateScale(10) * Matrix::CreateTranslation(10, 10, 0);
    Matrix m2 = Matrix::CreateScale(10) * Matrix::CreateTranslation(20, 20, 0);
    Matrix m3 = Matrix::CreateScale(10) * Matrix::CreateTranslation(30, 30, 0);
    Matrix m4 = Matrix::CreateScale(10) * Matrix::CreateTranslation(40, 40, 0);

    Rectangle2 r1(0, 0, 5, 5); //white
    Rectangle2 r2(5, 0, 5, 5); //green
    Rectangle2 r3(0, 5, 5, 5); //blue
    Rectangle2 r4(5, 5, 5, 5); //red

    sb->Begin();
        sb->Draw(t,m1, r1);
        sb->Draw(t,m2, r2);
        sb->Draw(t,m3, r3);
        sb->Draw(t,m4, r4);
    sb->End();
}
