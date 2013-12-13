#include "StdAfx2.h"
#include "Level.h"

#include "DeSerializer.h"

Level::Level(void)
{
}


Level::~Level(void)
{
}

void Level::Initialize(std::string resID, DeSerializer& s)
{
    s.BuildLevel(resID, levelStuff);
}