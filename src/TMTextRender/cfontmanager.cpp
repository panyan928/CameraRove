#include "cfontmanager.h"
namespace textRender {
    CFontManager::CFontManager()
    {
        m_fonts_size=0;
    }

    CFontManager::~CFontManager()
    {
        for (int i = 0; i < m_fonts_size; i++)
        {
            delete m_fonts[i];
            m_fonts[i] = 0x00;
        }

        m_fonts.clear();
        m_fonts_size=0;
    }

    int CFontManager::initialize(const char *path)
    {
        m_fonts_size=0;
        m_fontPath = path;
        return 0;
    }


    /**
     * @brief 
     * 
     * @param filename 
     * @param face 
     * @param tall 
     * @param bold 
     * @param italic 
     * @param antialias 
     * @return int 
     */
    int CFontManager::createFont(const char* filename, int face, int tall, bool bold, bool italic, bool antialias)
    {
        CFont* font = new CFont();

        if (font->create( filename, face, tall, bold, italic, antialias) != true)
        {
            if (font != 0x00) {
                delete font;
                font = 0x00;
            }
            return -1;
        }

        m_fonts.push_back(font);
        m_fonts_size++;

        return 0;
    }


    int CFontManager::getFontTall(int font_index)
    {
        int i = CONVERT_FONT_INDEX(font_index);

        if (i == -1)
        {
            return 0;
        }

        CFont *font = m_fonts[i];

        return font->getFontTall();
    }

    void CFontManager::renderChar(int font_index, char* text, Vec2i& position, Color color, float rotationAngle)
{

        int i = CONVERT_FONT_INDEX(font_index);

        if (i == -1)
        {
            printf("error font index!\n");
            return ;
        }

        CFont *font = m_fonts[i];
        font->setColor(color);
        font->renderChar(text, position, rotationAngle);
        //font->renderChar(text, position);
    }

    void CFontManager::renderChar(int font_index, char* text, Vec2i& position, Color color)
    {

        int i = CONVERT_FONT_INDEX(font_index);

        if (i == -1)
        {
            printf("error font index!\n");
            return;
        }

        CFont* font = m_fonts[i];
        font->setColor(color);
        font->renderChar(text, position);
        //font->renderChar(text, position);
    }

    void CFontManager::renderChar(int font_index, int code, Vec2i& position, Color color)
    {

        int i = CONVERT_FONT_INDEX(font_index);

        if (i == -1)
        {
            printf("error font index!\n");
            return ;
        }

        CFont *font = m_fonts[i];
        font->setColor(color);

        switch (code) {

        case 10: {
            position[1] += font->getFontTall();
            position[0]  = m_start[0];
            break;
        }
        default:
            glyphMetrics* metrics = new glyphMetrics;
            //font->renderChar(code, metrics, screen, position);
            if (metrics != 0x00)
                delete metrics;
            break;
        }

    }

    int CFontManager::getFontIndex(string name, int fontSize)
    {
        for (int i = 0; i < m_fonts_size; i++) {
            if (m_fonts[i]->getFontTall() == fontSize && name.compare(m_fonts[i]->fontName()) == 0)
                return i + 1;
        }
        string path = m_fontPath + name + ".TTF";
        createFont(path.c_str(), 0, fontSize, 0, 0, 1);
        return m_fonts_size;
    }

    int CFontManager::getFontIndex(string name, int fontSize, bool bold)
    {
        for (int i = 0; i < m_fonts_size; i++) {
            if (m_fonts[i]->getFontTall() == fontSize && name.compare(m_fonts[i]->fontName()) == 0
                && bold == m_fonts[i]->bold())
                return i + 1;
        }
        string path = m_fontPath + name + ".TTC";
        createFont(path.c_str(), 0, fontSize, bold, 0, 1);
        return m_fonts_size;
    }

    void CFontManager::setStartPosition(Vec2i pos)
    {
        memcpy(&m_start, &pos, sizeof(pos));
    }
    //syj
    int CFontManager::getCharWidth(int fontIndex, char c) {
        if (fontIndex >= 0 && fontIndex < static_cast<int>(m_fonts.size()) && m_fonts[fontIndex] != nullptr) {
            // ���� CFont �� .m_tall �� .size ��Ա�������� getFontTall ����ȫ
            int fontSize = getFontTall(fontIndex);
            return static_cast<int>(fontSize * 0.6f); // �ȿ�������ƿ���
        }
        return 8; // Ĭ�Ͽ���
    }

    int CFontManager::getFontHeight(int fontIndex) {
        if (fontIndex >= 0 && fontIndex < static_cast<int>(m_fonts.size()) && m_fonts[fontIndex] != nullptr) {
            return getFontTall(fontIndex); // �������к������ȫ
        }
        return 16; // Ĭ�ϸ߶�
    }
}


