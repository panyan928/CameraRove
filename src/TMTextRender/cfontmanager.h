#ifndef CFONTMANAGER_H
#define CFONTMANAGER_H

/**
  * @file     	cfont.h
  * @author   	MYL
  * @email   	yunlongma0@163.com
  * @version	V1.0
  * @date    	18-06-2021
  * @brief   	Font class based on FreeType
  * @attention
  *     This class is responsible for loading fonts, rendering \n
  *character bitmaps, and caching loaded characters
*/

#include "cfont.h"
#include <map>
#include "../TMUtil/cstringutil.h"
#define CONVERT_FONT_INDEX(x) (((x) < 1 || (x) > m_fonts_size) ? -1 : (x) - 1)
using namespace std;

namespace textRender {
  class CFontManager
  {
  public:
      CFontManager();
      ~CFontManager();
      
      int         initialize(const char* path);
      int         createFont(const char *filename, int face, int tall, bool bold, bool italic, bool antialias);//创建字体
      int         getFontTall(int font_index);
      void        renderChar(int font_index, int code, Vec2i& position, Color color);
      void        renderChar(int font_index, char* text, Vec2i& position, Color color, float rotationAngle = 0.0f);
     
      int         getFontIndex(string name, int fontSize);
      int         getFontIndex(string name, int fontSize, bool bold);
      
      void        setStartPosition(Vec2i pos);
      int         getFontHeight(int fontIndex);
      int         getCharWidth(int fontIndex, char c);

  private:
      vector<CFont*>  m_fonts;
      int             m_fonts_size;

      Vec2i           m_start;

      string          m_fontPath;
  };
}
#endif
