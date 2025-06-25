#ifndef CPAGE_H
#define CPAGE_H
//#include <GL/freeglut.h>
#include "gl\gl.h"	
#include "gl\glu.h"
typedef unsigned char byte;
class CPage
{
public:
    CPage();
    ~CPage();

    bool append(int wide, int tall, byte *rgba, float coords[]);
    GLuint getTexture(void);

private:
    GLuint     m_texture;
    int        m_wide;
    int        m_tall;
    int        m_posx;
    int        m_posy;
    int        m_maxCharTall;
};

#endif // CPAGE_H
