#include "text.h"

Text::Text(std::string text, int size, std::string index, DataType type)
{
    _text = text;
    _size = size;
    _index = index;
    _type = type;
}

Text::~Text()
{
}

void* Text::data()
{
    return const_cast<char*>(_text.c_str());
}
