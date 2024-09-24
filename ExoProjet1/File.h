#pragma once
#include <string>
#include <stdio.h>

class File
{
#pragma region Properties

public:
    FILE* _data;   
    std::string _path;   

#pragma endregion

#pragma region Methods

    File();  
    ~File(); 

    void Initialize(std::string path); 

    void Open(); 
    void Read(); 
    void Close(int* returnCode); 

#pragma endregion
};
