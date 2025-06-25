#ifndef CSYMBOL_H
#define CSYMBOL_H
#include <iostream>
namespace TMStyle {
class CSymbol
{
public:
    CSymbol();
    std::string symbolPath() const;
    int symbolWidth() const;
    int symbolHeight() const;

    void setSymbolPath(std::string path);
    void setSymbolSize(int width,int height);

private:
    std::string _symbolPath;
    int _symbolWidth;
    int _symbolHeight;
};
}

#endif // CSYMBOL_H
