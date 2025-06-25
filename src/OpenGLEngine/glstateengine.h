#ifndef GLSTATEENGINE_H
#define GLSTATEENGINE_H

//#include "GL/freeglut.h"
#include "gl\gl.h"	
#include "gl\glu.h"
#include <stack>
using namespace std;

class glStateEngine
{
public:
    glStateEngine();

    void turnOnPolygonState();
    void turnOnPolylineState();
    void turnOnPointState();
    void turnOnHudState();

private:
    bool _gl_cull_face;
    bool _gl_vertex_array;
    bool _gl_line_smooth;
    bool _gl_line_stipple;
    bool _gl_blend;
    bool _gl_depth_test;
    bool _gl_texture_2d;
    bool _gl_texture_coord_array;

    stack<int> _state;
};

#endif // GLSTATEENGINE_H
