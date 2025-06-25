#include "glstateengine.h"

glStateEngine::glStateEngine()
{

}

void glStateEngine::turnOnPolygonState()
{
    _gl_cull_face;
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnableClientState(GL_VERTEX_ARRAY);
}


