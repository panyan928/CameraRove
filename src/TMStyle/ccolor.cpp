#include "ccolor.h"
#include <math.h>

namespace TMStyle {
CColor::CColor()
{
    this->_color16 = "ffffff";
    this->_opacity = 1.0;
    this->_colorRGB[0] = 1.0;
    this->_colorRGB[1] = 1.0;
    this->_colorRGB[2] = 1.0;
}

CColor::CColor(std::string color16, float opacity)
{
    this->_color16 = color16;
    this->_opacity = opacity;
    hex2rgb();
}

CColor::CColor(float *colorRGB, float opacity)
{
    this->_colorRGB[0] = colorRGB[0];
    this->_colorRGB[1] = colorRGB[1];
    this->_colorRGB[2] = colorRGB[2];
    this->_opacity = opacity;
    rgb2hex();
}

std::string CColor::color16() const
{
    return _color16;
}

void CColor::hex2rgb()
{
    if(!this->_color16.size())
        return;
    std::string color = _color16;
    if (color[0] == '#')
        color = color.substr(1);
    //stoi函数将字符串转化为整型类型
    //int num = std::stoi(color.c_str(),NULL,16);
    const char* b = color.c_str();
	int len = color.size();
	int num = 0;
	for (int i = 0; i < len; i++) {
		num+=ASCII2HEX(b[i])*pow(16,len-i-1);
	}
    //16进制色彩表示中，两个数字一个完整的色彩部件（red，green，blue）
    //2进位存储表达中，8个字节位表达一个16进制整数
    //移位16个字节位，并执行&操作，可以得出red部件的数值
    int red = num>>16&0xFF;
    //移位8个字节位，并执行&操作，可以得出green部件的数值
    int green = num>>8&0xFF;
    //低位8个字节执行&操作，可以得出blue部件的数值
    int blue = num&0xFF;
    _colorRGB[0] = red/255.0;
    _colorRGB[1] = green/255.0;
    _colorRGB[2] = blue/255.0;
}

void CColor::rgb2hex()
{
    std::stringstream ss;
    ss << std::hex << (int(_colorRGB[0]*255) << 16 | int(_colorRGB[1]*255) << 8 | int(_colorRGB[2]*255));
    _color16 = ss.str();
}
float CColor::opacity() const
{
    return _opacity;
}

void CColor::colorRGB(float& r, float& g, float& b)
{
    r = _colorRGB[0];
    g = _colorRGB[1];
    b = _colorRGB[2];
}

void CColor::setColor16(std::string color)
{
    _color16 = color;
    hex2rgb();
}

void CColor::setColorRGB(float *color)
{
    this->_colorRGB[0] = color[0];
    this->_colorRGB[1] = color[1];
    this->_colorRGB[2] = color[2];
}

void CColor::setOpacity(float opacity)
{
    this->_opacity = opacity;
}

}

