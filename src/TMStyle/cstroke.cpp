#include "cstroke.h"
#include <stdlib.h>

namespace TMStyle {
CStroke::CStroke()
{
    _color = new CColor();
    _width = 1.0;
}

CStroke::~CStroke()
{
    if(_color!=0x00){
        delete _color;
        _color = 0x00;
    }
}

std::string CStroke::linecap() const
{
    return _linecap;
}

std::string CStroke::dashoffset() const
{
    return _dashoffset;
}

std::string CStroke::lineJoin() const
{
    return _lineJoin;
}

float CStroke::width() const
{
    return _width;
}

CColor *CStroke::getColor() const
{
    return _color;
}


void CStroke::setLinecap(std::string linecap)
{
    _linecap = linecap;
}

void CStroke::setDashoffset(std::string dashoffset)
{
    _dashoffset = dashoffset;
}

void CStroke::setLineJoin(std::string lineJoin)
{
    _lineJoin = lineJoin;
}

void CStroke::setWidth(float width)
{
    _width = width;
}

void CStroke::setDashArray(std::string dashArray)
{
    vector<string> ans;
    CStringUtil::split(dashArray, " ", ans);
    int len = ans.size();
    for (int i = 0; i < len; i++) {
        _dashArray.push_back(atof(ans[i].c_str()));
    }
}

vector<float> CStroke::dashArray() const
{
    return _dashArray;
}

}
