#include "dataunit.h"

DataUnit::DataUnit()
{
}

DataUnit::~DataUnit()
{
    int q = 0;
}

int DataUnit::size() const
{
    return _size;
}

DataType DataUnit::type() const
{
    return _type;
}

std::string DataUnit::index() const
{
    return _index;
}
