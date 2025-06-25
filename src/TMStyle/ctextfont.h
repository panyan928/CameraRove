#pragma once
#include <iostream>
namespace TMStyle {
    class CTextFont
    {
    public:
        CTextFont();
        //enum fontStyle { normal, italic, oblique };
        //enum fontWeight { normal, italic, oblique };
        std::string font() const; // ����
        int fontSize() const; // �ֺ�
        std::string fontStyle() const; // normal, italic, oblique
        std::string fontWeight() const; //normal, bold

        void setFont(std::string font_family);
        void setFontSize(int size);
        void setFontStyle(std::string style);
        void setFontWeight(std::string weight);

    private:
        std::string _font; // ����
        int _fontSize; // �ֺ�
        std::string _fontStyle; // normal, italic, oblique
        std::string _fontWeight; //normal, bold
    };
}

