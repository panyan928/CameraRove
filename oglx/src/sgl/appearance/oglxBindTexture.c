/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxBindTexture.c
 DESCRIPTION       : Call to glBindTexture if texture changes
 COPYRIGHT (C)     : 2010 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/
/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"
#include "mth.h"


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxUpdateColor
  DESCRIPTION:
    Function shall change current texture index if needed.
  SCOPE: global
  PARAMETERS:	    
    par_ui_index -> texture index, range: [0,glob_pr_sglStatemachine->ul_number_of_textures]
  RETURN:			
    void
---------------------------------------------------------------------+*/
void oglxBindTexture(GLuint par_ui_index)
{
    if (glob_pr_sglStatemachine->b_static_sequence_started) {
        glBindTexture(GL_TEXTURE_2D, par_ui_index);
    }
    else {
        GLuint loc_ui_ogl_texture_bound = glob_pr_sglStatemachine->ui_ogl_texture_bound;
        if (loc_ui_ogl_texture_bound == NO_OGL_TEXTURE_BOUND) {
            glBindTexture(GL_TEXTURE_2D, par_ui_index);
            glob_pr_sglStatemachine->ui_ogl_texture_bound = par_ui_index;
        }
        else {
            if (loc_ui_ogl_texture_bound != par_ui_index) {
                glBindTexture(GL_TEXTURE_2D, par_ui_index);
                glob_pr_sglStatemachine->ui_ogl_texture_bound = par_ui_index;
            }
            else {
                /* Nothing to do */
            }
        }
    }
    return;
}
