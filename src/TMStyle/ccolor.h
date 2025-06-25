#ifndef CCOLOR_H
#define CCOLOR_H
#include<iostream>
#include <sstream>
namespace TMStyle {
class CColor
{
public:
    CColor();
    CColor(std::string color16,float opacity);
    CColor(float* colorRGB,float opacity);
    std::string color16() const;
    float opacity() const;
    void colorRGB(float &r, float &g, float &b);

    void setColor16(std::string color);
    void setColorRGB(float* color);
    void setColorRGB(float r, float g, float b);
    void setOpacity(float opacity);

private:
    std::string _color16;
    float _colorRGB[3]; // 颜色值范围为 0-1
    float _opacity; // 透明度范围为0-1

    void hex2rgb();
    void rgb2hex();
        
    int ASCII2HEX(char cha) {
    	int res=0;
    	if (cha >= '0' && cha <= '9')
    		res = cha - 0x30;
    	else if (cha >= 'a' && cha <= 'f')
    		res = cha - 'a' + 10;
    	else if (cha >= 'A' && cha <= 'F')
    		res = cha - 'A' + 10;
    	else
    		res = 0;
    	return res;
    }
};
}
#endif // CCOLOR_H
