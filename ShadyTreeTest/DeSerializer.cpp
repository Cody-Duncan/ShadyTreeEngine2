#include "DeSerializer.h"
#include "DeSerializer\BaseDeserializer.h"
#include <jansson.h>
#include "Resources\GraphicsResourceHandles.h"
#include "Components\PhysicsComponent.h"
#include "GameLogic\CustomComponents\PlayerStateComponent.h"
#include "GameLogic\CustomComponents\AIComponent.h"
#include "GameLogic\CustomComponents\PowerupComponent.h"

DeSerializer::DeSerializer(void)
{
}


DeSerializer::~DeSerializer(void)
{
}

void getJsonStringFromFileRes(std::string resID, std::string& buffer)
{
    IResources& res = Resources::Instance();
    FileDataHandle fileH = res.LoadDataRes(resID);
    std::iostream* is = res.GetFileData(fileH);

    is->unsetf(std::ios::skipws); //don't skip whitespace
    buffer.assign( std::istream_iterator<char>(*is), std::istream_iterator<char>() ); //read into string

    res.CloseFile(fileH);
}

void parsePowerupElement(json_t* powerUp, PowerupComponent* pwrup_c)
{
    const char *key;
    json_t *value;
    json_object_foreach(powerUp, key, value)
    {
        if( sameKey(key, "type") )
        {
            pwrup_c->powerType = json_string_value( value );
        }
    }
}

GameObject* ParseArchetype(json_t* player)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    GameObject* go = GOC.Create();

    const char *key;
    json_t *value;
    json_object_foreach(player, key, value) 
    {
        json_t* component = json_object_get(player, key);
        if( sameKey(key, "Graphics") )
        {
            GraphicsComponent* gc = CF.createComponent<GraphicsComponent>();
            parseGraphics(component, gc);
            go->attachComponent(gc);
        }
        else if( sameKey(key, "Position") )
        {
            PositionalComponent* pc = CF.createComponent<PositionalComponent>();
            parsePosition(component, pc);
            go->attachComponent(pc);
        }
        else if( sameKey(key, "Physics") )
        {
            PhysicsComponent* phys_c = CF.createComponent<PhysicsComponent>();
            phys_c->clearCollideHandlers();
            parsePhysics(component, phys_c);
            go->attachComponent(phys_c);
        }
        else if (sameKey(key, "PlayerStateComponent"))
        {
            PlayerStateComponent* state = CF.createComponent<PlayerStateComponent>();
            go->attachComponent(state);
        }
        else if(sameKey(key, "PowerupComponent"))
        {
            PowerupComponent* powerUp = CF.createComponent<PowerupComponent>();
            parsePowerupElement(component, powerUp);
            go->attachComponent(powerUp);
        }
        else
        {
            DebugPrintf("Error: Tried to parse json object %s\n" , key);
        }
    }

    return go;
}

GameObject* ParseSubtype(json_t* subtypeObj)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    
    const char* basetype = json_string_value( json_object_get(subtypeObj, "Base") );
    if(!basetype)
        return nullptr;

    GameObject* go = GameObjectFactory::Instance().cloneArchetype(basetype);

    const char *key;
    json_t *value;
    json_object_foreach(subtypeObj, key, value) 
    {
        json_t* element = json_object_get(subtypeObj, key);
        if( sameKey(key, "Graphics") )
        {
            parseGraphics(element, go->getComponent<GraphicsComponent>());
        }
        else if( sameKey(key, "AI") )
        {
            AIComponent* ai_c =  CF.createComponent<AIComponent>();
            ai_c->aiType = json_string_value( element );
            go->attachComponent(ai_c);
        }
    }

    return go;
}

GameObject* ParsePowerupSub(json_t* subtypeObj)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    
    const char* basetype = json_string_value( json_object_get(subtypeObj, "Base") );
    if(!basetype)
        return nullptr;

    GameObject* go = GameObjectFactory::Instance().cloneArchetype(basetype);

    const char *key;
    json_t *value;
    json_object_foreach(subtypeObj, key, value) 
    {
        json_t* element = json_object_get(subtypeObj, key);
        if( sameKey(key, "Graphics") )
        {
            parseGraphics(element, go->getComponent<GraphicsComponent>());
        }
        else if( sameKey(key, "PowerupComponent") )
        {
            parsePowerupElement(element, go->getComponent<PowerupComponent>());
        }
    }

    return go;
}

GameObject* ParsePlatform(json_t* platform)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    GameObject* go = GOC.Create();

    const char *key;
    json_t *value;
    json_object_foreach(platform, key, value) 
    {
        json_t* component = json_object_get(platform, key);
        if( strcmp(key, "PrimitiveGraphics") == 0)
        {
            PrimitiveGraphicsComponent* pgc = CF.createComponent<PrimitiveGraphicsComponent>();
            parsePrimitiveGraphics(component, pgc);
            go->attachComponent(pgc);
        }
        else if( strcmp(key, "Position") == 0)
        {
            PositionalComponent* pc = CF.createComponent<PositionalComponent>();
            parsePosition(component, pc);
            go->attachComponent(pc);
        }
        else if( sameKey(key, "Physics") )
        {
            PhysicsComponent* phys_c = CF.createComponent<PhysicsComponent>();
            phys_c->clearCollideHandlers();
            parsePhysics(component, phys_c);
            go->attachComponent(phys_c);
        }
        else
        {
            DebugPrintf("Error: Tried to parse json object %s\n" , key);
        }
    }

    return go;
}

void OpenJson(std::string resID, json_t** root)
{
    std::string buffer;
    getJsonStringFromFileRes(resID, buffer);
    json_error_t error;
    
    //root = json_load_file("Player.json", 0, &error);
    *root = json_loadb(buffer.c_str(), buffer.size(), 0, &error);
    if(!*root)
    {
        DebugPrintf("\nPARSING ERROR: %s \nResource: %s, Line:%d, Position:%d\n", error.text, resID.c_str(), error.line, error.position);
        assert(root && "Parsing Error");
    }
}

void CloseJson(json_t* root)
{
    json_decref(root);
}

void DeSerializer::BuildArchetypes(std::string resID)
{
    DebugPrintf("GAME: Building Archetypes from resource: %s  File: %s\n", resID.c_str(), Resources::Instance().getFileSourceOfRes(resID).c_str());

    json_t* root;
    OpenJson(resID, &root);

    if(json_is_object(root))
    {
        const char *key;
        json_t *value;
        json_object_foreach(root, key, value) 
        {
            GameObject* newArch = nullptr;
            json_t* baseObject = json_object_get(root, key);

            newArch = ParseArchetype(baseObject);

            if(newArch != nullptr)
            {
                //deactivate the archetype and its components
                ComponentFactory& CF = ComponentFactory::Instance();
                newArch->active = false;
                for(auto iter = newArch->components.begin(); iter != newArch->components.end(); ++iter)
                {
                    CF.getComponent(iter->first, iter->second)->active = false;
                }

                //add to archetype list in factory
                GameObjectFactory::Instance().addArchetype(key, newArch->id);
            }
        }
    }
    else
    {
        DebugPrintf("ERROR: Archetypes did not parse \n");
    }

    CloseJson(root);

}

void DeSerializer::BuildSubtypes(std::string resID)
{
    DebugPrintf("GAME: Building Subtypes from resource: %s  File: %s\n", resID.c_str(), Resources::Instance().getFileSourceOfRes(resID).c_str());

    json_t* root;
    OpenJson(resID, &root);

    if(json_is_object(root))
    {
        const char *key;
        json_t *value;
        json_object_foreach(root, key, value) 
        {
            GameObject* newArch = nullptr;
            json_t* baseObject = json_object_get(root, key);

            newArch = ParseSubtype(baseObject);

            if(newArch != nullptr)
            {
                //deactivate the archetype and its components
                ComponentFactory& CF = ComponentFactory::Instance();
                newArch->active = false;
                for(auto iter = newArch->components.begin(); iter != newArch->components.end(); ++iter)
                {
                    CF.getComponent(iter->first, iter->second)->active = false;
                }

                //add to archetype list in factory
                GameObjectFactory::Instance().addArchetype(key, newArch->id);
                GameObjectFactory::Instance().addEnemyType(key);
            }
        }
    }

    CloseJson(root);

}

void DeSerializer::BuildPowerupTypes(std::string resID)
{
    DebugPrintf("GAME: Building Powerup types from resource: %s  File: %s\n", resID.c_str(), Resources::Instance().getFileSourceOfRes(resID).c_str());

    json_t* root;
    OpenJson(resID, &root);

    if(json_is_object(root))
    {
        const char *key;
        json_t *value;
        json_object_foreach(root, key, value) 
        {
            GameObject* newArch = nullptr;
            json_t* baseObject = json_object_get(root, key);

            newArch = ParsePowerupSub(baseObject);

            if(newArch != nullptr)
            {
                //deactivate the archetype and its components
                ComponentFactory& CF = ComponentFactory::Instance();
                newArch->active = false;
                for(auto iter = newArch->components.begin(); iter != newArch->components.end(); ++iter)
                {
                    CF.getComponent(iter->first, iter->second)->active = false;
                }

                //add to archetype list in factory
                GameObjectFactory::Instance().addArchetype(key, newArch->id);
                GameObjectFactory::Instance().addPowerupType(key);
            }
        }
    }

    CloseJson(root);
}

void DeSerializer::BuildLevel(std::string resID, std::vector<GameObject*>& levelObjects, Vector2& start, std::vector<Vector2>& startPositions)
{
    DebugPrintf("GAME: Building Level from resource: %s  File: %s\n", resID.c_str(), Resources::Instance().getFileSourceOfRes(resID).c_str());
    
    json_t* root;
    OpenJson(resID, &root);

    if(json_is_object(root))
    {
        const char *key;
        json_t *value;
        json_object_foreach(root, key, value) 
        {
            if(strcmp(key, "Platforms") == 0)
            {
                json_t* jarray = json_object_get(root, key);
                size_t index;
                json_t *value;

                json_array_foreach(jarray, index, value)
                {
                    levelObjects.push_back( ParsePlatform(value) );
                }
            }
            if(sameKey(key, "StartPosition" ))
            {
                double x, y;
                json_unpack(json_object_get(root, key), "[F,F]", &x, &y);
                start = Vector2((float)x,(float)y);
            }
            if(sameKey(key, "EnemyStartPositions" ))
            {
                json_t* jarray = json_object_get(root, key);
                size_t index;
                json_t *value;
                json_array_foreach(jarray, index, value)
                {
                    double x, y;
                    json_unpack(value, "[F,F]", &x, &y);
                    startPositions.push_back(Vector2((float)x,(float)y));
                }
                
            }
        }
    }

    CloseJson(root);
}