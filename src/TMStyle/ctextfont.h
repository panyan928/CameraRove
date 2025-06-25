#pragma once
#include <iostream>
namespace TMStyle {
    class CTextFont
    {
    public:
        CTextFont();
        //enum fontStyle { normal, italic, oblique };
        //enum fontWeight { normal, italic, oblique };
        std::string font() const; // ×ÖÌå
        int fontSize() const; // ×ÖºÅ
        std::string fontStyle() const; // normal, italic, oblique
        std::string fontWeight() const; //normal, bold

        void setFont(std::string font_family);
        void setFontSize(int size);
        void setFontStyle(std::string style);
        void setFontWeight(std::string weight);

    private:
        std::string _font; // ×ÖÌå
        int _fontSize; // ×ÖºÅ
        std::string _fontStyle; // normal, italic, oblique
        std::string _fontWeight; //normal, bold
    };
}

