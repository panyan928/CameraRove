#ifndef  OMAPGLOBAL_H
#define  OMAPGLOBAL_H

#include <string>
#include <iostream>
//#include <fstream>
#include "tmtools.h"
#include "../TMTextRender/cfontrender.h"
using namespace std;


//enum class consoleType {
//	_ERROR,
//	_WARNING,
//	_INFO,
//	_PRINT
//};
//
//extern consoleType consoleSettings;

extern int screenWidth;
extern int screenHeight;

extern int tileWidth;
extern int tileHeight;

extern int tileUnitSize;
extern int tileBufferSize;

enum class TileType {
	POLYGON,
	LINE,
	POINT,
	RASTER
};

extern textRender::CFontRender* _render;

extern int vCount;
extern int tCount;
extern int hudCount;


#endif // ! OMAPGLOBAL_H
