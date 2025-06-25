#include "stop.h"

Stop::Stop(int* stop, int size, std::string index, DataType type)
{
    _stop = stop;
    _size = size;
    _index = index;
    _type = type;
}

Stop::~Stop()
{
    if (_stop != 0x00)
        delete[] _stop;
    _stop = 0x00;
}

void* Stop::data()
{
    return _stop;
}
