#include "stdafx.h"
#include "FileResourcer.h"
#include <sstream>      // std::stringbuf

int FileResourcer::IDGen_file = 0;

int FileResourcer::generateFileID()
{
    return IDGen_file++;
}

FileResourcer::FileResourcer(void)
{
}

FileResourcer::~FileResourcer(void)
{
}

int FileResourcer::LoadStringData( char* data, FileDataHandle* h)
{
    std::iostream* stream = new std::iostream( new std::stringbuf(data));
    int id = generateFileID();
    if(fileStreams.size() <= (unsigned int)id)
        fileStreams.resize(id+1);

    fileStreams[id] = stream;
    h->gameDataID = id;

    return 0;
}

int FileResourcer::LoadFile(std::string filename, FileDataHandle* h)
{
    std::fstream* file = new std::fstream();
    file->open(filename);

    if(!file->is_open())
    {
        delete file;
        return 1;
    }

    int id = generateFileID();
    if(fileStreams.size() <= (unsigned int)id)
        fileStreams.resize(id+1);

    fileStreams[id] = file;
    fileNameToID[filename] = id;
    h->gameDataID = id;

    return 0;

}

std::iostream* FileResourcer::getFile(FileDataHandle h)
{
    return fileStreams[h.gameDataID];
}

void FileResourcer::closeFile(FileDataHandle h)
{
    std::iostream* stream = fileStreams[h.gameDataID];
    stream->flush();

    //close the file, if it is a file
    if(std::fstream* file = dynamic_cast<std::fstream*>(stream))
    {
        file->close();

        //remove from storage
        fileStreams[h.gameDataID] = 0;

        //remove filename reference
        std::string fileToErase;
        bool foundFile = false;
        for(auto iter = fileNameToID.begin(); iter != fileNameToID.end(); ++iter)
        {
            if(iter->second == h.gameDataID)
            {
                foundFile = true;
                fileToErase = iter->first;
            }
        }
        DebugAssert(foundFile, "How did program try to remove nonexistent file resource?");
        if(foundFile)
        {
            fileNameToID.erase(fileToErase);
        }
    }

    delete stream;
}

void FileResourcer::Dispose()
{
    //close all open file resources
    std::vector<FileDataHandle> toRemove;
    for(auto iter = fileNameToID.begin(); iter != fileNameToID.end(); ++iter)
    {
        FileDataHandle h = {iter->second};
        toRemove.push_back(h);
    }

    for(unsigned int i = 0; i < toRemove.size(); ++i)
    {
        closeFile(toRemove[i]);
    }

    fileStreams.clear();
    fileStreams.swap(std::vector<std::iostream*>());

    fileNameToID.clear();
    fileNameToID.swap(std::unordered_map<std::string, int>());
}

FileResourcer& FileResourcer::Instance()
{
    static FileResourcer fileRes;
    return fileRes;
}