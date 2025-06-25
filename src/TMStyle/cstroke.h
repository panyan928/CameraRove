#ifndef CSTROKE_H
#define CSTROKE_H
#include <iostream>
#include "ccolor.h"
#include <vector>
#include "../TMUtil/cstringutil.h"
using namespace std;

namespace TMStyle {
class CStroke
{
public:
    CStroke();
    virtual ~CStroke();
    std::string linecap() const;
    std::string dashoffset() const;
    std::string lineJoin() const;
    float width() const;
    CColor* getColor() const;

    void setLinecap(std::string linecap);
    void setDashoffset(std::string dashoffset);
    void setLineJoin(std::string lineJoin);
    void setWidth(float width);
    void setDashArray(std::string dashArray);
    vector<float> dashArray() const;

private:
    void hex2rgb();
    std::string _linecap;
    std::string _dashoffset;
    std::string _lineJoin;
    float _width;
    vector<float> _dashArray;

    CColor* _color;
};
}
#endif // CSTROKE_H
