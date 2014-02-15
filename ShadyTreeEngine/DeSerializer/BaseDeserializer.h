#include "jansson.h"
#include "Components\GraphicsComponent.h"
#include "Components\PositionalComponent.h"
#include "Components\PrimitiveGraphicsComponent.h"
#include "Components\PhysicsComponent.h"

ST_API extern void parseGraphics(json_t* obj, GraphicsComponent* c);
ST_API extern void parsePosition(json_t* obj, PositionalComponent* c);
ST_API extern void parsePrimitiveGraphics(json_t* root, PrimitiveGraphicsComponent* pgc);
ST_API extern void parsePhysics(json_t* root, PhysicsComponent* phys);

#define sameKey(k, t) strcmp(k, t) == 0