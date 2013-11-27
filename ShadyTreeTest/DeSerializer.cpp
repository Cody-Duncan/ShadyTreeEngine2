#include "DeSerializer.h"
#include <jansson.h>
#include "Resources.h"
#include "GraphicsResourceHandles.h"
#include "FileResourcer.h"
#include "DebugOutput.h"
#include <stdio.h>

DeSerializer::DeSerializer(void)
{
}


DeSerializer::~DeSerializer(void)
{
}

char* getJsonStringFromFileRes(std::string resID, int& length)
{
    IResources& res = Resources::Instance();
    FileDataHandle fileH = res.LoadDataRes(resID);
    std::iostream* is = res.GetFileData(fileH);

    // get length of file:
    is->seekg (0, is->end);
    int end = (int)is->tellg();
    is->seekg (0, is->beg);
    int beg = (int)is->tellg();
    length = end-beg;

    //read into buffer
    char * buffer = new char [length];
    is->read (buffer,length);

    res.CloseFile(fileH);

    return buffer;
}

void DeSerializer::BuildArchetypes(std::string resID)
{
    int bufferLen;
    char* buffer = getJsonStringFromFileRes(resID, bufferLen);

    json_t* root;
    json_error_t error;
    
    //root = json_load_file("Player.json", 0, &error);
    root = json_loadb(buffer, bufferLen, 0, &error);

    if(json_is_object(root))
    {
        const char *str;
        int vector[4];
        json_unpack(root, "{s:{s:{s:s,s:[i,i,i,i]}}}", "Player", "Graphics", "texture", &str, "textureArea", &vector[0], &vector[1], &vector[2], &vector[3]);

        const char *key;
        json_t *value;
        json_object_foreach(root, key, value) 
        {
            DebugPrintf(key);
        }
    }

    delete[] buffer;
}