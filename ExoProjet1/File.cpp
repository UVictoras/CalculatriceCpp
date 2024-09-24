#include "File.h"
#include <iostream>

File::File() : _data(nullptr)
{
}

File::~File()
{
    if (_data != nullptr)
    {
        fclose(_data);
        _data = nullptr;
    }
}

void File::Initialize(std::string path)
{
    _path = path;
}

void File::Open()
{
    errno_t err = fopen_s(&_data, _path.c_str(), "rb");

    if (err != 0 || _data == nullptr)
    {
        std::cerr << "Error opening file: " << _path << std::endl;
    }
    else
    {
        std::cout << "File opened successfully: " << _path << std::endl;
    }
}

void File::Read()
{
    if (_data == nullptr)
    {
        std::cerr << "File is not open!" << std::endl;
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), _data) != nullptr)
    {
        std::cout << buffer;
    }
}

void File::Close(int* returnCode)
{
    if (_data != nullptr)
    {
        *returnCode = fclose(_data);

        if (*returnCode == 0)
        {
            std::cout << "File closed successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error closing file." << std::endl;
        }

        _data = nullptr;
    }
    else
    {
        std::cerr << "File is already closed or was not opened." << std::endl;
    }
}
