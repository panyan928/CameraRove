/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglResetOpenGLSettings.c
 DESCRIPTION       : sglResetOpenGLSettings shall reset specific OpenGL settings
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglResetOpenGLSettings
  DESCRIPTION:
    Function shall reset specific OpenGL settings
  SCOPE: global
  PARAMETERS:		
    par_ul_settings -> SGL_RESET_ALL_SETTINGS to reset all settings or bitmask between values:
					SGL_DISABLE_BLENDING
					SGL_DISABLE_TEXTURE_2D
					SGL_DISABLE_SCISSOR_TEST
					SGL_DISABLE_DEPTH_TEST
					SGL_DISABLE_CULL_FACE
					SGL_RESET_BLEND_FUNC
					SGL_DISABLE_LIGHTING
					SGL_ENABLE_LINE_SMOOTH
					SGL_ENABLE_VERTEX_ARRAY
					SGL_ENABLE_TEXTURE_ARRAY
					SGL_RESET_TEXTURE_ENV
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglResetOpenGLSettings(SGLulong par_ul_settings)
{
    if ((par_ul_settings & SGL_DISABLE_BLENDING) == SGL_DISABLE_BLENDING) {
        glDisable(GL_BLEND);
    }

#ifndef ES2_DEV_ENV
    if ((par_ul_settings & SGL_DISABLE_TEXTURE_2D) == SGL_DISABLE_TEXTURE_2D) {
        glDisable(GL_TEXTURE_2D);
    }
#endif

    if ((par_ul_settings & SGL_DISABLE_SCISSOR_TEST) == SGL_DISABLE_SCISSOR_TEST) {
        glDisable(GL_SCISSOR_TEST);
    }

    if ((par_ul_settings & SGL_DISABLE_DEPTH_TEST) == SGL_DISABLE_DEPTH_TEST) {
        glDisable(GL_DEPTH_TEST);
    }

    if ((par_ul_settings & SGL_DISABLE_CULL_FACE) == SGL_DISABLE_CULL_FACE) {
        glDisable(GL_CULL_FACE);
    }

    if ((par_ul_settings & SGL_RESET_BLEND_FUNC) == SGL_RESET_BLEND_FUNC) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

#ifndef ES2_DEV_ENV
    if ((par_ul_settings & SGL_DISABLE_LIGHTING) == SGL_DISABLE_LIGHTING) {
        glDisable(GL_LIGHTING);
    }

    if ((par_ul_settings & SGL_ENABLE_LINE_SMOOTH) == SGL_ENABLE_LINE_SMOOTH) {
        glEnable(GL_LINE_SMOOTH);
    }

    if ((par_ul_settings & SGL_ENABLE_VERTEX_ARRAY) == SGL_ENABLE_VERTEX_ARRAY) {
        glEnableClientState(GL_VERTEX_ARRAY);
    }

    if ((par_ul_settings & SGL_ENABLE_TEXTURE_ARRAY) == SGL_ENABLE_TEXTURE_ARRAY) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if ((par_ul_settings & SGL_RESET_TEXTURE_ENV) == SGL_RESET_TEXTURE_ENV) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
    }
#else

    if ((par_ul_settings & SGL_ENABLE_VERTEX_ARRAY) == SGL_ENABLE_VERTEX_ARRAY) {
        glEnableVertexAttribArray(OGLX_ES2_VERTEX_ARRAY);
    }

    if ((par_ul_settings & SGL_ENABLE_TEXTURE_ARRAY) == SGL_ENABLE_TEXTURE_ARRAY) {
        glEnableVertexAttribArray(OGLX_ES2_TEX_COORD_ARRAY);
    }

#endif

    return;
}

/* End of File ***************************************************************/
