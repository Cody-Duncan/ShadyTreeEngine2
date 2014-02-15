#pragma once

#include <iostream>
#include <fstream>
#include "Resources\GraphicsResourceHandles.h"

class FileResourcer
{
public:
    std::vector<std::iostream*> fileStreams;
    std::unordered_map<std::string, int> fileNameToID;

    int LoadStringData( char* data, FileDataHandle* h);
    int LoadFile(std::string filename, FileDataHandle* h);
    std::iostream* getFile(FileDataHandle h);
    void closeFile(FileDataHandle h);
    void Dispose();

    static FileResourcer& Instance();

private:
    static int IDGen_file;
    int generateFileID();

    //Singleton
    FileResourcer(void);
    ~FileResourcer(void);
    FileResourcer(FileResourcer const&);              
    void operator=(FileResourcer const&); 
};

