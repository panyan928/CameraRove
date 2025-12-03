/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxDisable.c
 DESCRIPTION       : Call to glDisable if activation changes
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

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxDisable
  DESCRIPTION:
    Function shall disable given OpenGL mode if it was not already disabled.
  SCOPE: global
  PARAMETERS:
    par_e_Mode -> OpenGL mode which can be GL_TEXTURE_2D, GL_BLEND, GL_STENCIL_TEST or GL_SCISSOR_TEST
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxDisable(SGLulong par_e_Mode)
{
    if (glob_pr_sglStatemachine->b_static_sequence_started) {
        glDisable(par_e_Mode);
    }
    else {
        switch (par_e_Mode) {

        case GL_TEXTURE_2D:
        {
            /* If GL_TEXTURE_2D is not already disabled, disable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d) {
#ifndef ES2_DEV_ENV
                glDisable(GL_TEXTURE_2D);
#endif
                glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }
        }
            break;

        case GL_STENCIL_TEST:
        {
            /* If GL_STENCIL_TEST is not already disabled, disable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test) {
                glDisable(GL_STENCIL_TEST);
                glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }
        }
            break;

        case GL_SCISSOR_TEST:
        {
            /* If GL_SCISSOR_TEST is not already disabled, disable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test) {
                glDisable(GL_SCISSOR_TEST);
                glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }

        }
            break;

        case GL_DEPTH_TEST:
        {
            /* If GL_DEPTH_TEST is not already enabled, enable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test) {
                glDisable(GL_DEPTH_TEST);
                glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }
        }
            break;

        default:
            /*
               case GL_BLEND:
             */
        {
            /* If GL_BLEND is not already disabled, disable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_blend) {
                glDisable(GL_BLEND);
                glob_pr_sglStatemachine->r_ogl_state.b_state_blend = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }
        }
            break;

        }
    }
    return;
}

/* End of File ***************************************************************/
