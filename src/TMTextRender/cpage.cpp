#include "cpage.h"

CPage::CPage()
{
    m_wide = m_tall = 256;
    m_posx = m_posy = 0;

   // In a line, for a max height character
   m_maxCharTall = 0;

   glGenTextures(1, &m_texture);    // Using your API here
   glBindTexture(GL_TEXTURE_2D, m_texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wide, m_tall, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0X00);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

CPage::~CPage()
{

}

bool CPage::append(int wide, int tall, byte *rgba, float coords[])
{
    if (m_posy + tall > m_tall)
    {
        // not enough line space in this page
        return false;
    }

   // If this line is full ...
    if (m_posx + wide > m_wide)
    {
        int newLineY = m_posy + m_maxCharTall;

       if (newLineY + tall > m_tall)
        {
            // No more space for new line in this page, should allocate a new one
            return false;
        }

        // Begin in new line
        m_posx = 0;
        m_posy = newLineY;
        // Reset
        m_maxCharTall = 0;
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, m_posx, m_posy, wide, tall, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

    coords[0] = m_posx / (float)m_wide;                // left
    coords[3] = m_posy / (float)m_tall;                // top
    coords[2] = (m_posx + wide) / (float)m_wide;    // right
    coords[1] = (m_posy + tall) / (float)m_tall;    // bottom

    m_posx += wide;

    if (tall > m_maxCharTall)
    {
        m_maxCharTall = tall;
    }

    return true;
}

GLuint CPage::getTexture()
{
    return m_texture;
}
