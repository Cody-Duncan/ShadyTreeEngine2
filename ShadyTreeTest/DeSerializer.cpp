#include "DeSerializer.h"
#include "BaseDeserializer.h"
#include <jansson.h>
#include "GraphicsResourceHandles.h"

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

GameObject* ParsePlayer(json_t* player)
{
    GameObjectCache& GOC = GameObjectCache::Instance();
    ComponentFactory& CF = ComponentFactory::Instance();
    GameObject* go = GOC.Create();

    const char *key;
    json_t *value;
    json_object_foreach(player, key, value) 
    {
        json_t* component = json_object_get(player, key);
        if( strcmp(key, "Graphics") == 0)
        {
            GraphicsComponent* gc = CF.createComponent<GraphicsComponent>();
            parseGraphics(component, gc);
            go->attachComponent(gc);
        }
        else if( strcmp(key, "Position") == 0)
        {
            PositionalComponent* pc = CF.createComponent<PositionalComponent>();
            parsePosition(component, pc);
            go->attachComponent(pc);
        }
        else
        {
            DebugPrintf("Error: Tried to parse json object %s" , key);
        }
    }

    return go;
}

void DeSerializer::BuildArchetypes(std::string resID)
{
    std::string buffer;
    getJsonStringFromFileRes(resID, buffer);

    json_t* root;
    json_error_t error;
    
    //root = json_load_file("Player.json", 0, &error);
    root = json_loadb(buffer.c_str(), buffer.size(), 0, &error);

    if(json_is_object(root))
    {
        const char *key;
        json_t *value;
        json_object_foreach(root, key, value) 
        {
            json_t* baseObject = json_object_get(root, key);
            GameObject* newArch = ParsePlayer(baseObject);
                
            //deactivate the archetype
            ComponentFactory& CF = ComponentFactory::Instance();
            newArch->active = false;
            for(auto iter = newArch->components.begin(); iter != newArch->components.end(); ++iter)
            {
                CF.getComponent(iter->first, iter->second)->active = false;
            }
            
            GameObjectFactory::Instance().addArchetype(key, newArch->id);
        }
    }

}