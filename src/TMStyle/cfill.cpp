#include "cfill.h"
namespace TMStyle {
CFill::CFill()
{
    _color = new CColor();
}

CFill::~CFill()
{
    if(_color!=0x00){
        delete _color;
        _color = 0x00;
    }
}

CColor *CFill::getColor() const
{
    return _color;
}

}
