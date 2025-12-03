/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEndStaticSequence.c
 DESCRIPTION       : sglEndStaticSequence shall end a sequence of static objects 
 COPYRIGHT (C)     : 2014 by Esterel Technologies. All Rights Reserved.
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
  NAME: sglEndStaticSequence
  DESCRIPTION:
    Function shall end a sequence of static objects 
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEndStaticSequence(void)
{
#ifdef OGLX_DISPLAY_LISTS
    glEndList();
#endif

#ifdef OGLX_FBO
    GLuint loc_ui_framebuffer = (GLuint) glob_pr_sglStatemachine->ul_framebuffer;

    oglxPopMatrix();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDeleteFramebuffers(1, &loc_ui_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

    sglDisable(SGL_MODULATE);

    glob_pr_sglStatemachine->b_static_sequence_started = SGL_FALSE;
    return;
}

/* End of File ***************************************************************/
