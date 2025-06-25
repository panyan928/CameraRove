#include "ctextfont.h"
namespace TMStyle {
    CTextFont::CTextFont()
    {
    }
    std::string CTextFont::font() const
    {
        return _font;
    }

    int CTextFont::fontSize() const
    {
        return _fontSize;
    }

    std::string CTextFont::fontStyle() const
    {
        return _fontStyle;
    }

    std::string CTextFont::fontWeight() const
    {
        return _fontWeight;
    }
    void CTextFont::setFont(std::string font_family)
    {
        _font = font_family;
    }
    void CTextFont::setFontSize(int size)
    {
        _fontSize = size;
    }
    void CTextFont::setFontStyle(std::string style)
    {
        _fontStyle = style;
    }
    void CTextFont::setFontWeight(std::string weight)
    {
        _fontWeight = weight;
    }
}