#include "image.h"

Image::Image(unsigned char* data, int size, std::string index, DataType type)
{
    _data = data;
    _size = size;
    _index = index;
}

Image::~Image()
{
    if (_data != 0x00)
        delete[] _data;
    _data = 0x00;
}

void* Image::data()
{
    return _data;
}

void Image::setInfo(int height, int width, int nrChannels)
{
    _height = height;
    _width = width;
    _nrChannels = nrChannels;

}

void Image::getInfo(int& height, int& width, int& nrChannels) const
{
    height = _height;
    width = _width;
    nrChannels = _nrChannels;
}
