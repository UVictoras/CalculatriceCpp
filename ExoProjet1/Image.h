#pragma once
#include "File.h"
#include <Windows.h>

class Image
{
#pragma region Properties

public:
    tagBITMAPFILEHEADER     _bitmapFileHeaderData;
    tagBITMAPINFOHEADER     _bitmapInfoHeaderData;

    BYTE* _bmpBuffer;
    BYTE* _bgr;

    size_t                  _result;
    File* _file;

#pragma endregion

#pragma region Methods

    Image();
    ~Image();

    void InitializeImage(std::string path);
    void InitializeFile(std::string path);

    void Load();
    FILE* CreateImage();
    void CreateBuffer(int* returnCode);
    void ReadBMP();
    void CreateBitmap();

#pragma endregion
};
