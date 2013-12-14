#include "StdAfx2.h"
#include "Level.h"

#include "DeSerializer.h"
#include "PhysicsComponent.h"
#include "PositionalComponent.h"


Level::Level(void)
{
}


Level::~Level(void)
{
}

void Level::Initialize(std::string resID, DeSerializer& s, int _Height, int _Width, float _BorderRatio)
{
    s.BuildLevel(resID, levelStuff, playerStart, enemyStartPositions);

    float borderHeight = _Height *_BorderRatio - _Height;
    float borderWidth  = _Width * _BorderRatio - _Width;

    LevelSpace.extents = Vector2( _Width+borderWidth*2, _Height+borderHeight*2);
    LevelPos = Vector2( LevelSpace.extents.x/2 - borderWidth, LevelSpace.extents.y/2 - borderHeight);

}

/// <summary>
/// Determines whether a body at the given position is on a platform.
/// </summary>
/// <param name="position">The position of the body.</param>
/// <param name="body">The body.</param>
/// <returns>true if the body is on a platform</returns>
bool Level::IsOnPlatform(Vector2 position, BB* body)
{
    for(unsigned int i = 0; i < levelStuff.size(); ++i)
    {
        //get the platform body and position
        BB* platformBody = levelStuff[i]->getComponent<PhysicsComponent>()->body;
        Vector2& platformPos = levelStuff[i]->getComponent<PositionalComponent>()->position;

        //get the height of the body
        float height = 0;
        switch(body->type)
        {
        case(Circle):
            height = static_cast<BB_Circle*>(body)->radius;
            break;
        case(Rect):
            height = static_cast<BB_Rectangle*>(body)->extents.y/2;
            break;
        }

        //check if a point 1 unit below the body collides with a platform.
        if(PointCollisionCheck(Vector2(position.x, position.y + 1.0f + height), platformBody, platformPos))
            return true;
    }
    return false;
}

bool Level::IsOutsideLevel(Vector2 position)
{
    return !PointCollisionCheck(position, &LevelSpace, LevelPos);
}