#include "ctext.h"
namespace TMStyle {
CText::CText()
{
    _color = new CColor("#000000", 1);
    _font = new CTextFont();

    _anchorPointX = 0;
    _anchorPointY = 0;
    _displacementX = 0;
    _displacementY = 0;
}

CText::~CText()
{
    if(_color != 0x00){
        delete _color;
        _color = 0x00;
    }
    if (_font != 0x00) {
        delete _font;
        _font = 0x00;
    }
}

CTextFont* CText::font() const
{
   return _font;
}

std::string CText::label() const
{
    return _label;
}

void CText::getDisplayment(int &x, int &y)
{
    x = _displacementX;
    y = _displacementY;
}

void CText::setDisplayment(int x, int y)
{
    _displacementX = x;
    _displacementY = y;
}

void CText::getAnchorPoint(int &x, int &y)
{
    x = _anchorPointX;
    y = _anchorPointY;
}

void CText::setAnchorPoint(int x, int y)
{
    _anchorPointX = x;
    _anchorPointY = y;
}


void CText::setLabel(std::string label)
{
    _label = label;
}

CColor *CText::getColor() const
{
    return _color;
}

}
