#include "BaseDeserializer.h"
#include "Resources.h"

#define sameKey(k, t) strcmp(k, t) == 0

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
            json_unpack(json_object_get(root, key), "[f,f,f,f]", &r, &g, &b, &a);
            pgc->color = Color((float)r,(float)g,(float)b,(float)a);
        }
        else if(sameKey(key, "center"))
        {
            int x, y;
            json_unpack(json_object_get(root, key), "[i,i]", &x, &y);
            pgc->center = Vector2((float)x,(float)y);
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

#define CLOCKWISE_VERTS_PRIMITIVE
            #if !defined(COUNTERCLOCKWISE_VERTS_PRIMITIVE) && !defined(CLOCKWISE_VERTS_PRIMITIVE)
            #define COUNTERCLOCKWISE_VERTS_PRIMITIVE
            #endif
            #ifdef  COUNTERCLOCKWISE_VERTS_PRIMITIVE
            pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
            pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[3]));  //bottom left
            pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right

            pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
            pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right
            pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[1]));  //top right

            #endif
            #ifdef CLOCKWISE_VERTS_PRIMITIVE
            pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
            pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[1]));  //top right
            pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right

            pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[1]));  //top left
            pgc->triangleListPoints.push_back(Vector2(pointCoords[2], pointCoords[3]));  //bottom right
            pgc->triangleListPoints.push_back(Vector2(pointCoords[0], pointCoords[3]));  //bottom left
            #endif

        }
    }
}