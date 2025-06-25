#ifndef CFILL_H
#define CFILL_H
#include <iostream>
#include "ccolor.h"
namespace TMStyle {
    class CFill
    {
    public:
        CFill();
        virtual ~CFill();
        CColor* getColor() const;
    private:
        CColor* _color;
    };
}


#endif // CFILL_H
