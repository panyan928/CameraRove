#include <stdio.h>
#include <stdlib.h>
#include "mapDrv.h"
#include "cfont.h"
#include "gl\gl.h"	
#include "gl\glu.h"
namespace textRender {
CFont::CFont()
{
   /*m_outline = false;
   m_outline_color = Color(0, 0, 0, 255);
   m_bg_color  = Color(0, 0, 0, 0);
   m_charBuffers.setCapacity(1000);*/
}

CFont::~CFont()
{
    FT_Error err;

    if (m_face)
    {
        if ((err = FT_Done_Face(m_face)) != FT_Err_Ok)
        {
            printf("FT_Done_Face() Error %d\n", err);
        }
        m_face = NULL;
    }

    //m_chars.clear();

}
/**
 * @brief               load font
 * @details             
 *     Loads a font from a file based on the font attributes \n 
 * passed in and stores it in an array of fonts in CfontManager
 * @param library       Font library, initialize before use
 * @param filename      The font path
 * @param face_index    font face, Usually 0
 * @param tall          font tall in pixels, also known as font size 
 * @param bold          bold or not
 * @param italic        italic or not
 * @param antialias     antialias or not
 * @return true         create successfully
 * @return false        Font creation failed
 */
bool CFont::create(const char* filename, FT_Long face_index, int tall, bool bold, bool italic, bool antialias)
{     
    string path = filename;
    m_name = CStringUtil::getNameFromSrc(path);
    oglfCreateFont(filename, m_name.c_str(), tall, &m_font);  

    m_antialias = antialias;
    m_bold = bold;
    m_tall = tall;

    return true;
}


int CFont::getFontTall() const
{
    return m_tall;
}

/**
 * @brief           Render the character
 * @details 
 *      According to the input pixel coordinate position, character \n
 * number and  character metrics, the bitmap data of the character is \n
 * copied into a large pixel matrix for the convenience of a subsequent \n
 * rendering.
 * @param code      character code
 * @param metrics   character metrics
 * @param screen    pixel matrix
 * @param position  pixel position of the character
 */
void CFont::renderChar(int code, glyphMetrics *metrics, unsigned char *screen, Vec2i& position)
{
    /*
    map<int, CChar*>::iterator iter;
    iter = m_chars.find(code);
    if (iter != m_chars.end()) {
        Draw_Bitmap((*iter).second, screen, position);
        return;
    }
    loadChar(code, metrics, screen, position);*/

    // get data from the buffer
    CChar* pChar = m_charBuffers.getBufferData(code);
    // find it
    if (pChar != 0x00) {
        if (pChar->type() == Gray)
            Draw_Bitmap(pChar, screen, position);
        if (pChar->type() == Mono)
            Draw_MONO_Bitmap(pChar, screen, position);
        return;
    }
    loadChar(code, metrics, screen, position);
}

void CFont::renderChar(char* text, Vec2i& position)
{    
#if 0
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewportWidth = viewport[2];
    int viewportHeight = viewport[3];
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport[2], 0, viewport[3], -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Flong����
    glColor4f(m_color[0] / 255.0, m_color[1] / 255.0, m_color[2] / 255.0, m_color[3] / 255.0);
    //glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);
    //oglfDrawString(m_font, position[0], 768 - position[1], (const unsigned char*)text, FONT_JUST_HLEFT, FONT_JUST_VTOP, false);
    oglfDrawString(m_font, position[0], 768 - position[1], (const unsigned char*)text, FONT_JUST_HLEFT, FONT_JUST_VTOP);
    glPopMatrix();
#else
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    //Flong����
    glColor4f(m_color[0] / 255.0, m_color[1] / 255.0, m_color[2] / 255.0, m_color[3] / 255.0);
    //glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);

    //oglfDrawString(m_font, position[0], viewport[3] - position[1], (const unsigned char*)text, FONT_JUST_HLEFT, FONT_JUST_VBOTTOM);
    oglfDrawString(m_font, position[0], viewport[3] - position[1], (const unsigned char*)text, FONT_JUST_HCENTER, FONT_JUST_VCENTER);
#endif
}
void CFont::setColor(Color color)
{
    memcpy(&m_color, &color, sizeof(m_color));
}

string CFont::fontName() const
{
    return m_name;
}

bool CFont::bold() const
{
    return m_bold;
}


/**
 * @brief               Draws Gray mode characters to the pixel matrix
 * @details         
 *      From glyphs to load layout data and bitmap data, \n
 * copy to pixel matrix and cache
 * @param glyph         font glyph
 * @param screen        pixel matrix
 * @param position      pixel position
 * @param charBuffer    character buffer
 */
void CFont::Draw_Bitmap(FT_GlyphSlot& glyph, unsigned char* screen, Vec2i& position, CChar* charBuffer) {
    FT_Bitmap *bitmap = (&glyph->bitmap);

    if (bitmap == NULL) {
        printf("bitmap error!\n");
        return;
    }

    int span = glyph->metrics.horiAdvance / 64;
    int posX = glyph->metrics.horiBearingX / 64;
    int posY = m_tall - glyph->metrics.horiBearingY / 64;

    int width = glyph->metrics.width / 64;
    int height = glyph->metrics.height / 64;

    unsigned char* data = charBuffer->getOrCreateBuffer(width * height);

    for (int i = 0; i < height; i++) {
        int index = (posY + i + position[1]) * 1024 * 4 + (posX + position[0]) * 4;
        if (index >= 1024 * 768 * 4)
            break;
        for (int j = 0; j < width; j++) {
            int color = bitmap->buffer[i * width + j];
            data[i * width + j] = color;
            if (color == 0)
            {
                screen[index++] = m_bg_color[3];
                screen[index++] = m_bg_color[2];
                screen[index++] = m_bg_color[1];
                screen[index++] = m_bg_color[0];

                //cout << " ";
            }
            else
            {
                if(m_outline && isFontEdge(bitmap->buffer, i, j, width, height))
                {
                    screen[index++] = m_outline_color[3];
                    screen[index++] = m_outline_color[2];
                    screen[index++] = m_outline_color[1];
                    screen[index++] = m_outline_color[0];

                    //cout<< "+";
                }
                else
                {
                    screen[index++] = color * 1.0 / 255 * m_color[3];
                    screen[index++] = color * 1.0 / 255 * m_color[2];
                    screen[index++] = color * 1.0 / 255 * m_color[1];
                    screen[index++] = color * 1.0 / 255 * m_color[0];
                    //cout << "*";
                }
            }
        }
    }

   // cout << endl;

    position[0] += span;

    glyphMetrics* metrics = new glyphMetrics;


    metrics->width = glyph->metrics.width ;
    metrics->height = glyph->metrics.height;

    metrics->horiBearingX = glyph->metrics.horiBearingX;
    metrics->horiBearingY = glyph->metrics.horiBearingY;
    metrics->horiAdvance  = glyph->metrics.horiAdvance;
    metrics->vertBearingX = glyph->metrics.vertBearingX;
    metrics->vertBearingY = glyph->metrics.vertBearingY;

    charBuffer->setInfo(metrics);

    if (metrics != 0x00)
        delete metrics;

    m_charBuffers.addBuffer(charBuffer->code(), charBuffer);
    /*
    if ((m_rear + 1) > m_capacity)
        m_chars.insert({charBuffer->code(), charBuffer});
    else {

    }*/
    //writePngFile("./test.png", data, span, 30, 8);
}

/**
 * @brief               Draws Gray mode characters to the pixel matrix
 * @details         
 *      From buffer to load layout data and bitmap data, \n
 * copy to pixel matrix
 * @param screen        pixel matrix
 * @param position      pixel position
 * @param charBuffer    character buffer
 */
void CFont::Draw_Bitmap(CChar *charBuffer, unsigned char *screen, Vec2i &position)
{
    if (charBuffer == 0x00)
        return;

    glyphMetrics metrics;
    charBuffer->getInfo(&metrics);

    int span = metrics.horiAdvance / 64;
    int posX = metrics.horiBearingX / 64;
    int posY = m_tall - metrics.horiBearingY / 64;

    int width = metrics.width / 64;
    int height = metrics.height / 64;

    unsigned char* data = charBuffer->getOrCreateBuffer(0);

    for (int i = 0; i < height; i++) {
        int index = (posY + i + position[1]) * 1024 * 4 + (posX + position[0]) * 4;
        if (index >= 1024 * 768 * 4)
            break;
        for (int j = 0; j < width; j++) {
            int color = data[i * width + j];

            if (color == 0)
            {
                screen[index++] = m_bg_color[3];
                screen[index++] = m_bg_color[2];
                screen[index++] = m_bg_color[1];
                screen[index++] = m_bg_color[0];

                //cout << " ";
            }
            else if(m_outline && isFontEdge(data, i, j, width, height))
            {
                screen[index++] = 255;
                screen[index++] = 255;
                screen[index++] = 255;
                screen[index++] = 255;

                //cout<< "+";
            }
            else
            {
                screen[index++] = color*1.0/255*m_color[3];
                screen[index++] = color*1.0/255*m_color[2];
                screen[index++] = color*1.0/255*m_color[1];
                screen[index++] = color*1.0/255*m_color[0];

                //cout << "*";
            }
        }
        //cout << endl;
    }
    position[0] += span;
}


/**
 * @brief               Draws MONO mode characters to the pixel matrix
 * @details         
 *      From glyphs to load layout data and bitmap data, \n
 * copy to pixel matrix and cache
 * @param glyph         font glyph
 * @param screen        pixel matrix
 * @param position      pixel position
 * @param charBuffer    character buffer
 */
void CFont::Draw_MONO_Bitmap(FT_GlyphSlot& glyph, unsigned char* screen, Vec2i& position, CChar* charBuffer)
{
    FT_Bitmap *bitmap = (&glyph->bitmap);

    if (bitmap == NULL) {
        printf("bitmap error!\n");
        return;
    }
    //int span = glyph->metrics.horiAdvance / 64;
    int posX = glyph->metrics.horiBearingX / 64;


    int posY = m_tall - glyph->metrics.horiBearingY / 64;

    int width = glyph->metrics.width / 64;
    int height = glyph->metrics.height / 64;

    unsigned char* data = charBuffer->getOrCreateBuffer(width * height);

    for (int i = 0; i < height; i++) {
        int index = (posY + i + position[1]) * 1024 * 4 + (posX + position[0]) * 4;
        if (index >= 1024 * 768 * 4)
            break;
        for (int j = 0; j < width; j++) {
            // bitmap.width  位图宽度
            // bitmap.rows   位图行数（高度）
            // bitmap.pitch  位图一行占用的字节�?
            int color = bitmap->buffer[i * bitmap->pitch + j / 8];

            if (!(color & (1 << (7 - (j % 8)))))
            {
                data[i * width + j] = 0;
                screen[index++] = 0;
                screen[index++] = 0;
                screen[index++] = 0;
                screen[index++] = 0;

                //cout << " ";
            }
            else
            {
                data[i * width + j] = 1;

                screen[index++] =  m_color[3];
                screen[index++] =  m_color[2];
                screen[index++] =  m_color[1];
                screen[index++] =  m_color[0];

                //cout << "*";
            }
        }
        //cout << endl;
    }

    glyphMetrics* metrics = new glyphMetrics;


    metrics->width = glyph->metrics.width ;
    metrics->height = glyph->metrics.height;

    metrics->horiBearingX = glyph->metrics.horiBearingX;
    metrics->horiBearingY = glyph->metrics.horiBearingY;
    metrics->horiAdvance  = glyph->metrics.horiAdvance;
    metrics->vertBearingX = glyph->metrics.vertBearingX;
    metrics->vertBearingY = glyph->metrics.vertBearingY;

    charBuffer->setInfo(metrics);

    if (metrics != 0x00)
        delete metrics;

    m_charBuffers.addBuffer(charBuffer->code(), charBuffer);
}

/**
 * @brief               Draws MONO mode characters to the pixel matrix
 * @details         
 *      From buffer to load layout data and bitmap data, \n
 * copy to pixel matrix
 * @param screen        pixel matrix
 * @param position      pixel position
 * @param charBuffer    character buffer
 */
void CFont::Draw_MONO_Bitmap(CChar *charBuffer, unsigned char *screen, Vec2i &position)
{
    if (charBuffer == 0x00)
        return;

    glyphMetrics metrics;
    charBuffer->getInfo(&metrics);

    int span = metrics.horiAdvance / 64;
    int posX = metrics.horiBearingX / 64;
    int posY = m_tall - metrics.horiBearingY / 64;

    int width = metrics.width / 64;
    int height = metrics.height / 64;

    unsigned char* data = charBuffer->getOrCreateBuffer(0);

    for (int i = 0; i < height; i++) {
        int index = (posY + i + position[1]) * 1024 * 4 + (posX + position[0]) * 4;
        if (index >= 1024 * 768 * 4)
            break;
        for (int j = 0; j < width; j++) {
            int color = data[i * width + j];

            if (color == 0)
            {
                screen[index++] = m_bg_color[3];
                screen[index++] = m_bg_color[2];
                screen[index++] = m_bg_color[1];
                screen[index++] = m_bg_color[0];

                cout << " ";
            }
            else if(m_outline && isFontEdge(data, i, j, width, height))
            {
                screen[index++] = 255;
                screen[index++] = 255;
                screen[index++] = 255;
                screen[index++] = 255;

                cout<< "+";
            }
            else
            {
                screen[index++] = m_color[3];
                screen[index++] = m_color[2];
                screen[index++] = m_color[1];
                screen[index++] = m_color[0];

                cout << "*";
            }
        }
        cout << endl;
    }
    position[0] += span;
}


/**
 * @brief Reads character data from glyphs and caches it
 * @param code          character code 
 * @param metrics       character metrics 
 * @param screen        pixel matrix        
 * @param position      pixel position
 */
void CFont::loadChar(int code, glyphMetrics *metrics, unsigned char* screen, Vec2i& position)
{

    FT_Error err;
    FT_UInt glyph_index = FT_Get_Char_Index(m_face, (FT_ULong)code);
    // glyph_index is valid
    if (glyph_index > 0)
    {
        if ((err = FT_Load_Glyph(m_face, glyph_index, FT_OUTLINE_HIGH_PRECISION)) == FT_Err_Ok)
        {
            // get font glyph
            FT_GlyphSlot glyph = m_face->glyph;

            FT_Render_Mode render_mode = m_antialias ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

            if (m_antialias && m_bold)
            {
                if ((err = FT_Outline_EmboldenXY(&glyph->outline, 64, 64)) != FT_Err_Ok)
                {
                    printf("FT_Outline_EmboldenXY() Error %d\n", err);
                }
            }

            if ((err = FT_Render_Glyph(glyph, render_mode)) == FT_Err_Ok)
            {
                // get bitmap
                FT_Bitmap *bitmap = &glyph->bitmap;

                switch (bitmap->pixel_mode)
                {
                    //MONO模式�?个像素仅�?bit保存，只有黑和白�?
                    case FT_PIXEL_MODE_MONO:
                    {
                        if (!m_antialias && m_bold)
                        {
                            /*if ((err = FT_Bitmap_Embolden(m_library, bitmap, 60, 0)) != FT_Err_Ok)
                            {
                                printf("FT_Bitmap_Embolden() Error %d\n", err);
                            }*/
                        }

                        CChar* tm_char = new CChar(code, Mono);
                        Draw_MONO_Bitmap(glyph, screen, position, tm_char);
                        //Draw_Bitmap(glyph, screen, position, tm_char);
                        break;
                    }
                    //GRAY模式1个像素用1个字节保存�?
                    case FT_PIXEL_MODE_GRAY:
                    {
                        CChar* tm_char = new CChar(code, Gray);
                        Draw_Bitmap(glyph, screen, position, tm_char);
                        //Draw_Bitmap(glyph->bitmap.buffer, glyph->bitmap.pitch,
                            //glyph->bitmap.rows, data, 30);

                        //ConvertGRAYToRGBA(bitmap, m_rgba);
                        break;
                    }
                    default:
                    {
                        //memset(m_rgba, 0xFF, m_rgbaSize);
                        break;
                    }
                }

                return;
            }
            else
            {
                printf("FT_Render_Glyph() Error %d\n", err);
            }
        }
        else
        {
            printf("FT_Load_Glyph() Error %d\n", err);
        }
    }

    memset(metrics, 0, sizeof(glyphMetrics));

    return ;
}

bool CFont::isFontEdge(unsigned char *buffer, int row, int col, int width,int height){

    if(row == 0 || row == height-1) return true;
    else if(col == 0 || col == width-1) return true;

    else if(buffer[(row-1) * width + col]==0) return true;
    else if(buffer[(row+1) * width+ + col]==0) return true;
    else if(buffer[row * width + col-1]==0) return true;
    else if(buffer[row * width + col+1]==0) return true;
    else return false;
}


void CFont::setOutline(bool type)
{
    this->m_outline = type;
}

void CFont::setOutlineColor(Color color)
{
    this->m_outline_color = color;
}

}