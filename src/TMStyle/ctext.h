#ifndef CTEXT_H
#define CTEXT_H
#include "ccolor.h"
#include <iostream>
#include "ctextfont.h"
namespace TMStyle {
class CText
{
public:
    CText();
    virtual ~CText();
    CTextFont* font() const;
    std::string label() const;
    CColor* getColor() const;

    void getDisplayment(int& x, int& y);
    void setDisplayment(int x, int y);
    void getAnchorPoint(int& x, int& y);
    void setAnchorPoint(int x, int y);
    void setLabel(std::string label);

private:
    std::string _label;

    float _anchorPointX;
    float _anchorPointY;
    float _displacementX;
    float _displacementY;
    
    CColor* _color;
    CTextFont* _font;
};
}
#endif // CTEXT_H
