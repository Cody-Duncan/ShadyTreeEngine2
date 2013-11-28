#include "jansson.h"
#include "GraphicsComponent.h"
#include "PositionalComponent.h"

ST_API extern void parseGraphics(json_t* obj, GraphicsComponent* c);
ST_API extern void parsePosition(json_t* obj, PositionalComponent* c);