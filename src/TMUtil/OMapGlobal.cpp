#include "OMapGlobal.h"

//consoleType consoleSettings = consoleType::_ERROR;

int screenWidth =1024;
int screenHeight = 768;

int tileHeight = 3;
int tileWidth = 4;

int tileUnitSize = 256;
int tileBufferSize = 5;

textRender::CFontRender* _render = new textRender::CFontRender(screenWidth, screenHeight);

int vCount = 0;
int tCount = 0;
int hudCount = 0;
