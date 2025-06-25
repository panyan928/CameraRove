#include "heights.h"


Heights::Heights(short int* heights, int size, std::string index, DataType type)
{
    _heights = heights;
    _size = size;
    _index = index;
    _type = type;
}

Heights::~Heights()
{
    if (_heights != 0x00)
        delete[] _heights;
    _heights = 0x00;
}

void* Heights::data()
{
    return _heights;
}
