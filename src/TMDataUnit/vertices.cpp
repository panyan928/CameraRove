#include "vertices.h"



Vertices::Vertices(float* pts, int size, std::string index, DataType type)
{
    _pts = pts;
    _size = size;
    _index = index;
    _type = type;
}

Vertices::~Vertices()
{
    if (_pts != 0x00)
        delete[] _pts;
    _pts = 0x00;
}

void* Vertices::data()
{
    return _pts;
}
