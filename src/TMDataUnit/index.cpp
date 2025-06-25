#include "index.h"

Index::Index(int* indexData, int size, std::string index, DataType type)
{
	_indexData = indexData;
    _size = size;
    _index = index;
    _type = type;
}

Index::~Index()
{
    if (_indexData != 0x00)
        delete[] _indexData;
    _indexData = 0x00;
}

void* Index::data()
{
    return _indexData;
}
