#ifndef MAPDRV_H
#define MAPDRV_H

#define FONT_JUST_HLEFT    0x0 /* Horizontal justification: left */
#define FONT_JUST_HCENTER  0x1 /* Horizontal justification: center */
#define FONT_JUST_HRIGHT   0x2 /* Horizontal justification: right */

#define FONT_JUST_VBOTTOM  0x2 /* Vertical justification: bottom */
#define FONT_JUST_VCENTER  0x1 /* Vertical justification: center */
#define FONT_JUST_VTOP     0x0 /* Vertical justification: top */

extern int isRoma;

void Initial(void);
void drawFontE(float x,float y,char* a);
void drawText(void) ;
void myDisplay1(void);
void myDisplay(void);
void Display2d();
void* romaFucn(void* args);
void drawEarth(void);
void drawEarth1(void);
void drawRaster(int zoom, int row, int col);
void drawRasters(int zoom, int beginRow, int endRow, int beginCol, int endCol);
void renderFontArray();
void romaCreate();
void drawAirplaneIcon(float aircraftLat, float aircraftLon, float aircraftHeading,
    int screenWidth, int screenHeight, float currentZoom);
#endif

