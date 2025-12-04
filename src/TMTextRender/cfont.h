#ifndef TEXTRENDER_CFONT_H
#define TEXTRENDER_CFONT_H

/**
  * @file     	cfont.h
  * @author   	MYL   yunlongma0@163.com
  * @version	V1.0
  * @date    	18-06-2021
  * @brief   	Font class based on FreeType
  * @attention
  *     This class is responsible for loading fonts, rendering \n
  *character bitmaps, and caching loaded characters
*/

#include "freetype/ft2build.h"
#include <vector>
#include "cchar.h"
#include <map>
#include <iostream>

#include "../TMUtil/cstringutil.h"
#include "cbuffer.h"
#include "InputCApi.h"

using namespace std;

#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_OUTLINE_H

// 全局变量声明
extern bool g_isAirportMode;
      
namespace textRender {
  class CFont
  {
  public:
      CFont();
      virtual ~CFont();

      bool                    create(const char *filename, FT_Long face_index, int tall, bool bold, bool italic, bool antialias);

      int                     getFontTall(void) const;

      void                    renderChar(int code, glyphMetrics *metrics, unsigned char* screen, Vec2i& position);
      void                    renderChar(char* text, Vec2i& position, float rotationAngle = 0.0f);
      //void                    renderChar(char* text, Vec2i& position);
      void                    setColor(Color color);
      void                    setOutline(bool type);
      void                    setOutlineColor(Color color);
      bool                    isAirportPath();
      string                  fontName() const;
      bool                    bold() const;
  private:

      void                    loadChar(int code, glyphMetrics *metrics, unsigned char* screen, Vec2i& position);

      void                    Draw_Bitmap(FT_GlyphSlot& glyph, unsigned char* screen, Vec2i& position, CChar* charBuffer);
      void                    Draw_Bitmap(CChar* charBuffer, unsigned char* screen, Vec2i& position);

      void                    Draw_MONO_Bitmap(FT_GlyphSlot& glyph, unsigned char* screen, Vec2i& position, CChar* charBuffer);
      void                    Draw_MONO_Bitmap(CChar* charBuffer, unsigned char* screen, Vec2i& position);

      bool                    isFontEdge(unsigned char*  buffer,int row, int col,int width,int height);

      //typedef                 std::map<int, CChar *> TCharMap;
      //TCharMap                m_chars;
      CBuffer<int, CChar>     m_charBuffers;                  /**< char buffer. */


      FT_Face                 m_face;                         /**< font face. */

      bool                    m_antialias;                    /**< font property: antialias. */
      bool                    m_bold;                         /**< font property: bold. */
      bool                    m_outline;                      /**< font property: outline. */

      Color                   m_color;                        /**< font color: font. */
      Color                   m_outline_color;                /**< font color: outline. */
      Color                   m_bg_color;                     /**< font color: background. */

      int                     m_tall;                         /**< font property: tall (or size). */
      string                  m_name;                         /**< font property: font name. */

      JTFONT                  m_font;
  };
}
#endif
