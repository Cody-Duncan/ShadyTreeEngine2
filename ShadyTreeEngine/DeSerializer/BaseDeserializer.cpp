#include "BaseDeserializer.h"
#include "Resources\Resources.h"
#include "Components\PhysicsComponent.h"



void parseGraphics(json_t* root, GraphicsComponent* gc)
{
    const char *key;
    json_t *value;
    json_object_foreach(root, key, value) 
    {
        if(sameKey(key, "texture"))
        {
            const char* resName = json_string_value( json_object_get(root, key) );
            gc->texture = Resources::Instance().LoadTextureRes(resName);
        }
        else if(sameKey(key, "textureArea"))
        {
            int x, y, width, height;
            json_unpack(json_object_get(root, key), "[i,i,i,i]", &x, &y, &width, &height);
            gc->textureArea.position.x = (float)x;
            gc->textureArea.position.y = (float)y;
            gc->textureArea.dimensions.x = (float)width;
            gc->textureArea.dimensions.y = (float)height;
        }
    }
}

void parsePosition(json_t* root, PositionalComponent* pc)
{
    const char *key;
    json_t *value;
    json_object_foreach(root, key, value) 
    {
        if(sameKey(key, "position"))
        {
            int x, y;
            json_unpack(json_object_get(root, key), "[i,i]", &x, &y);
            pc->position = Vector2((float)x,(float)y);
        }
        else if(sameKey(key, "rotation"))
        {
            pc->rotation = (float) json_real_value( json_object_get(root, key));
        }
        else if(sameKey(key, "scale"))
        {
            pc->scale = (float) json_real_value( json_object_get(root, key));
        }
        else if(sameKey(key, "rotationCentered"))
        {
            json_unpack(json_object_get(root, key), "b", &pc->rotationCentered);
        }
    }
}

void parsePrimitiveGraphics(json_t* root, PrimitiveGraphicsComponent* pgc)
{
    const char *key;
    json_t *value;
    json_object_foreach(root, key, value) 
    {
        if(sameKey(key, "layer"))
        {
            pgc->layer = (int) json_integer_value( json_object_get(root, key));
        }
        else if(sameKey(key, "color"))
        {
            double r,g,b,a;
            json_unpack(json_object_get(root, key), "[F,F,F,F]", &r, &g, &b, &a);
            pgc->color = Color((float)r,(float)g,(float)b,(float)a);
        }
        else if(sameKey(key, "center"))
        {
            //int x, y;
            //json_unpack(json_object_get(root, key), "[i,i]", &x, &y);
            //pgc->center = Vector2((float)x,(float)y);
        }
        else if(sameKey(key, "triangle"))
        {
            json_t* jarray = json_object_get(root, key);
            size_t index;
            json_t *value;
            std::vector<float> pointCoords;

            json_array_foreach(jarray, index, value) {
                pointCoords.push_back( (float)json_integer_value(value) );
            }

            for(unsigned int i = 0; i < pointCoords.size(); i+=2)
            {
                pgc->triangleListPoints.push_back(Vector2(pointCoords[i], pointCoords[i+1]));
            }
        }
        else if(sameKey(key, "square"))
        {
            json_t* jarray = json_object_get(root, key);
            size_t index;
            json_t *value;
            std::vector<float> pointCoords;

            json_array_foreach(jarray, index, value) {
                pointCoords.push_back( (float)json_integer_value(value) );
            }

            #if !defined(COUNTERCLOCKWISE_VERTS_PRIMITIVE) && !defined(CLOCKWISE_VERTS_PRIMITIVE)
            #define CLOCKWISE_VERTS_PRIMITIVE
            #endif

            #ifdef CLOCKWISE_VERTS_PRIMITIVE
                //Note: These are clockwise bound, orthogonal transform makes them counterclockwise in vertex shader.
                pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
                pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[1]));  //top right
                pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right

                pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
                pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right
                pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[3]));  //bottom left
            #endif

            #ifdef  COUNTERCLOCKWISE_VERTS_PRIMITIVE
                pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
                pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[3]));  //bottom left
                pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right

                pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
                pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right
                pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[1]));  //top right

            #endif
            
            pgc->center = Vector2(pointCoords[2]/2, pointCoords[3]/2);

        }
    }
}

void parsePhysics(json_t* root, PhysicsComponent* phys)
{
    const char *key;
    json_t *value;
    json_object_foreach(root, key, value) 
    {
        if(sameKey(key, "static"))
        {
            phys->IsStatic = json_is_true(json_object_get(root, key));
        }
        else if(sameKey(key, "mass"))
        {
            phys->Mass = (float)json_real_value(json_object_get(root, key));
            phys->InvMass = floatCompare(phys->Mass, 0.0f) ? 0.0f : 1.0f/phys->Mass;
        }
        else if(sameKey(key, "velocity"))
        {
            double x, y;
            json_unpack(json_object_get(root, key), "[F,F]", &x, &y);
            phys->velocity = Vector2((float)x,(float)y);
        }
        else if(sameKey(key, "acceleration"))
        {
            double x, y;
            json_unpack(json_object_get(root, key), "[F,F]", &x, &y);
            phys->acceleration = Vector2((float)x,(float)y);
        }
        else if(sameKey(key, "restitution"))
        {
            phys->Restitution = (float)json_real_value(json_object_get(root, key));
        }
        else if(sameKey(key, "body"))
        {
            json_t* body = json_object_get(root, key);
            if(json_t* circle = json_object_get(body, "circle"))
            {
                BB_Circle* circ = new BB_Circle();
                if(json_t* rad = json_object_get(circle, "radius"))
                {
                    circ->radius = (float)json_real_value( rad );
                }
                else
                {
                    DebugPrintf("PARSING ERROR: Physics body circle did not contain \"radius\". \n");
                }
                phys->body = circ;
            }
            else if(json_t* rectangle = json_object_get(body, "rectangle"))
            {
                BB_Rectangle* rect = new BB_Rectangle();
                json_t* extent;
                if( extent = json_object_get(rectangle, "dimension") )
                {
                    double x, y;
                    json_unpack(extent, "[F,F]", &x, &y);
                    rect->extents = Vector2((float)x,(float)y);
                }
                else if( extent = json_object_get(rectangle, "dimensions") )
                {
                    double x, y;
                    json_unpack(extent, "[F,F]", &x, &y);
                    rect->extents = Vector2((float)x,(float)y);
                }
                else
                {
                    DebugPrintf("PARSING ERROR: Physics body rectangle did not contain \"dimension\". \n");
                }
                phys->body = rect;
            }
        }
    }
}