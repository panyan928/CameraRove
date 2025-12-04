#include "cfontrender.h"
#include "gl\gl.h"  
#include "gl\glu.h"
#define M_PI 3.141592653f
namespace textRender {
    CFontRender::CFontRender(int screenWidth, int screenHeight)
    {
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;
        m_screen = new unsigned int[(screenHeight) * (screenWidth) * 5];
        memset(m_screen, 0, screenHeight * screenWidth * 5 * sizeof(unsigned int));
    }

    CFontRender::~CFontRender()
    {
        if (m_screen != 0x00)
            delete[] m_screen;
        m_screen = 0x00;
        if (m_fontManager != 0x00)
            delete m_fontManager;
        m_fontManager = 0x00;
    }

    int CFontRender::initialize(const char* path)
    {
        m_fontManager = new CFontManager;
        if (m_fontManager->initialize(path) == 0) {
            return 0;
        }

        printf("error while initializing m_fontManager!\n");

        if (m_fontManager != 0x00)
            delete m_fontManager;
        m_fontManager = 0x00;
        return -1;
    }

    void CFontRender::loadFont(const char *path, int face, int tall, bool bold, bool italic, bool antialias)
    {
        if (m_fontManager == 0x00) {
            //printf("m_fontManager is invalid!\n");
            return ;
        }

        m_fontManager->createFont(path, face, tall, bold, italic, antialias);
    }
#if 0
    int CFontRender::render(wstring text, Vec2i pos, int fontSize, string fontName, Color fontColor)
    {
        if (m_fontManager == 0x00) {
            //printf("m_fontManager is invalid!\n");
            return -1;
        }

        int fontIndex = m_fontManager->getFontIndex(fontName, fontSize);


        size_t size = text.size();

        m_fontManager->setStartPosition(pos);

        for (size_t i = 0; i < size; i++) {
            m_fontManager->renderChar(fontIndex, text[i], m_screen, pos, fontColor);
        }

        return 0;
    }    

    int CFontRender::render(wstring text, Vec2i pos, int fontSize, string fontName, Color fontColor, bool bold)
    {
        if (m_fontManager == 0x00) {
            //printf("m_fontManager is invalid!\n");
            return -1;
        }

        int fontIndex = m_fontManager->getFontIndex(fontName, fontSize, bold);


        size_t size = text.size();

        m_fontManager->setStartPosition(pos);

        for (size_t i = 0; i < size; i++) {
            m_fontManager->renderChar(fontIndex, text[i], m_screen, pos, fontColor);
        }

        return 0;
    }

    int CFontRender::render(wstring text, Vec2i pos, int fontIndex, Color fontColor)
    {
        size_t size = text.size();

        m_fontManager->setStartPosition(pos);

        for (size_t i = 0; i < size; i++) {
            m_fontManager->renderChar(fontIndex, text[i], m_screen, pos, fontColor);
        }

        return 0;
    }
#endif
    int CFontRender::render(char* text, Vec2i pos, int fontSize, string fontName, Color fontColor)
    {
        if (m_fontManager == 0x00) {
            //printf("m_fontManager is invalid!\n");
            return -1;
        }

        int fontIndex = m_fontManager->getFontIndex(fontName, fontSize);


        //size_t size = wcslen(text);

        m_fontManager->setStartPosition(pos);

        // 获取当前地图旋转角度
        float modelview[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
        float rotationAngle = atan2(modelview[1], modelview[0]) * 180.0f / M_PI;

        m_fontManager->renderChar(fontIndex, text, pos, fontColor, rotationAngle);


        return 0;
    }

    int CFontRender::render(string text, Vec2i pos, int fontSize, string fontName, Color fontColor)
    {
        if (m_fontManager == 0x00) {
            //printf("m_fontManager is invalid!\n");
            return -1;
        }

        int fontIndex = m_fontManager->getFontIndex(fontName, fontSize);


        size_t size = text.size();

        m_fontManager->setStartPosition(pos);

        for (size_t i = 0; i < size; i++) {
            m_fontManager->renderChar(fontIndex, text[i], pos, fontColor);
        }

        return 0;
    }

    int CFontRender::render(string text, Vec2i pos, int fontIndex, Color fontColor)
    {
        size_t size = text.size();

        m_fontManager->setStartPosition(pos);

        for (size_t i = 0; i < size; i++) {
            m_fontManager->renderChar(fontIndex, text[i], pos, fontColor);
        }

        return 0;
    }

    //int writePng2File(const char* png_file_name, unsigned char*  pixels , int width, int height, int bit_depth)
    //{
    //    png_structp png_ptr;
    //    png_infop info_ptr;
    //    FILE *png_file = fopen(png_file_name, "wb");
    //    if (!png_file)
    //    {
    //        return -1;
    //    }
    //    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    //    if(png_ptr == NULL)
    //    {
    //        printf("ERROR:png_create_write_struct/n");
    //        fclose(png_file);
    //        return 0;
    //    }
    //    info_ptr = png_create_info_struct(png_ptr);
    //    if(info_ptr == NULL)
    //    {
    //        printf("ERROR:png_create_info_struct/n");
    //        png_destroy_write_struct(&png_ptr, NULL);
    //        return 0;
    //    }
    //    png_init_io(png_ptr, png_file);
    //    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, /*PNG_COLOR_TYPE_PALETTE*/PNG_COLOR_TYPE_RGB_ALPHA,
    //        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    //
    //
    //    png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    //    if (!palette) {
    //        fclose(png_file);
    //        png_destroy_write_struct(&png_ptr, &info_ptr);
    //        return false;
    //    }
    //    png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
    //    png_write_info(png_ptr, info_ptr);
    //    png_set_packing(png_ptr);
    //    //这里就是图像数据了
    //    png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    //    /*
    //    for (int i = 0; i < height; ++i)
    //    {
    //        rows[i] = (png_bytep)(pixels + (i) * width * 4);
    //    }*/
    //    int pos = 0;
    //    int temp = (4 * width);
    //           for (int i = 0; i < height; i ++) {
    //               rows[i] = (png_bytep)malloc(temp* sizeof(unsigned char));
    //               for (int j = 0; j < temp; j += 4) {
    //                   rows[i][j+3] = pixels[pos++];
    //                   rows[i][j+2] = pixels[pos++];
    //                   rows[i][j+1] = pixels[pos++];
    //                   rows[i][j+0] = pixels[pos++];
    //               }
    //           }
    //    png_write_image(png_ptr, rows);
    //    delete[] rows;
    //    png_write_end(png_ptr, info_ptr);
    //    png_free(png_ptr, palette);
    //    palette=NULL;
    //    png_destroy_write_struct(&png_ptr, &info_ptr);
    //    fclose(png_file);
    //    return 0;
    //}

    void CFontRender::exportScreenImage(const char *path)
    {
        //writePng2File(path, m_screen, screenWidth, screenHeight, 8);
    }

    CFontManager *CFontRender::getFontManager() const
    {
        return m_fontManager;
    }

    void CFontRender::clearScreen()
    {
        memset(m_screen, 0, _screenWidth * _screenHeight * 5 * sizeof(unsigned int));
    }

    unsigned int *CFontRender::getData() const
    {
        return m_screen;
    }
}
