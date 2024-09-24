#include "Image.h"
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <Windows.h>
#include <cstring>

Image::Image() : _bmpBuffer(nullptr), _bgr(nullptr), _result(0), _file(nullptr)
{
}

Image::~Image()
{
    if (_bmpBuffer != nullptr)
    {
        delete[] _bmpBuffer;
        _bmpBuffer = nullptr;
    }

    if (_file != nullptr)
    {
        delete _file;
        _file = nullptr;
    }
}

void Image::InitializeImage(std::string path)
{
    InitializeFile(path);
}

void Image::InitializeFile(std::string path)
{
    _file = new File();
    _file->Initialize(path);
}

void Image::Load()
{
    _file->Open();
}

FILE* Image::CreateImage()
{
    FILE* newImage = nullptr;
    errno_t err = fopen_s(&newImage, _file->_path.c_str(), "wb");

    if (err != 0 || newImage == nullptr)
    {
        std::cerr << "Error creating image file: " << _file->_path << std::endl;
        return nullptr;
    }

    return newImage;
}

void Image::CreateBuffer(int* returnCode)
{
    *returnCode = fseek(_file->_data, 0, SEEK_END);
    size_t fileLen = ftell(_file->_data);
    *returnCode = fseek(_file->_data, 0, SEEK_SET);

    _bmpBuffer = new BYTE[fileLen];
    _result = fread(_bmpBuffer, sizeof(BYTE), fileLen, _file->_data);
}

void Image::ReadBMP()
{
    Load();
    int returnCode;
    CreateBuffer(&returnCode);
    _file->Close(&returnCode);
}

void Image::CreateBitmap()
{
    memcpy(&_bitmapFileHeaderData, _bmpBuffer, sizeof(tagBITMAPFILEHEADER));
    memcpy(&_bitmapInfoHeaderData, _bmpBuffer + sizeof(tagBITMAPFILEHEADER), sizeof(tagBITMAPINFOHEADER));
    _bgr = _bmpBuffer + sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER);
}
