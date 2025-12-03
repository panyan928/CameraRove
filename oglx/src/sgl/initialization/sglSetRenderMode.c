/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetRenderMode.c
 DESCRIPTION       : sglSetRenderMode command shall select the rendering mode 
					for line drawing: smooth lines or raw lines
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

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetRenderMode
  DESCRIPTION:
    Function shall select the rendering mode for line drawing: smooth lines or raw lines.
  SCOPE: global
  PARAMETERS:
    par_b_mode -> Mode to draw OpenGL lines (SGL_RAW_OPENGL_LINES or SGL_SMOOTH_LINES)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetRenderMode(SGLbyte par_b_mode)
{
    if (glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX) {

        /* Store new render mode in global OGLX context */
        if (par_b_mode == SGL_RAW_OPENGL_LINES) {
            glob_pr_sglStatemachine->b_line_rendering_mode = SGL_RAW_OPENGL_LINES;
        }
        else {
            if (par_b_mode == SGL_SMOOTH_LINES) {
                glob_pr_sglStatemachine->b_line_rendering_mode = SGL_SMOOTH_LINES;
                glob_pr_sglStatemachine->f_last_ogl_linewidth = 0.0F;
            }
            else {
                /* Nothing to do */
            }
        }
    }
    else {
        /* Error if function is called between sglBegin/sglEnd */
        oglxSetError(SGL_ERROR_SGL_SETRENDERMODE, 0U);
    }

    return;
}

/* End of File ***************************************************************/
