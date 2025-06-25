#include "csymbol.h"
namespace TMStyle {
CSymbol::CSymbol()
{
    this->_symbolHeight = 16;
    this->_symbolWidth = 16;
}

std::string CSymbol::symbolPath() const
{
    return _symbolPath;
}

int CSymbol::symbolWidth() const
{
    return _symbolWidth;
}

int CSymbol::symbolHeight() const
{
    return _symbolHeight;
}

void CSymbol::setSymbolPath(std::string path)
{
    _symbolPath = path;
}

void CSymbol::setSymbolSize(int width, int height)
{
    _symbolHeight = height;
    _symbolWidth = width;
}

}

