/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxEnable.c
 DESCRIPTION       : Call to glEnable if the activation changes
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
  NAME: oglxTexEnvi
  DESCRIPTION:
    Function shall change given texture parameters if they have changed
  SCOPE: global
  PARAMETERS:		
    par_ul_target -> texture target parameter (identical to parameter of glTexEnvi)
	par_ul_pname -> texture pname parameter (identical to parameter of glTexEnvi)
	par_ul_param -> texture param parameter (identical to parameter of glTexEnvi)
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglxTexEnvi(SGLulong par_ul_target, SGLulong par_ul_pname, SGLlong par_l_param)
{
    if (glob_pr_sglStatemachine->r_ogl_state.ul_texture_target == par_ul_target && glob_pr_sglStatemachine->r_ogl_state.ul_texture_pname == par_ul_pname &&
        glob_pr_sglStatemachine->r_ogl_state.l_texture_param == par_l_param) {
        /*nothing to do */
    }
    else {
#ifndef ES2_DEV_ENV
        glTexEnvi((GLenum) par_ul_target, (GLenum) par_ul_pname, (GLint) par_l_param);
        glob_pr_sglStatemachine->r_ogl_state.ul_texture_target = par_ul_target;
        glob_pr_sglStatemachine->r_ogl_state.ul_texture_pname = par_ul_pname;
        glob_pr_sglStatemachine->r_ogl_state.l_texture_param = par_l_param;
#endif
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxEnable
  DESCRIPTION:
    Function shall enable given OpenGL mode if it was not already enabled.
  SCOPE: global
  PARAMETERS:		
    par_e_Mode -> OpenGL mode which can be GL_TEXTURE_2D, GL_BLEND, GL_STENCIL_TEST or GL_SCISSOR_TEST
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglxEnable(SGLulong par_e_Mode)
{
    if (glob_pr_sglStatemachine->b_static_sequence_started) {
        glEnable(par_e_Mode);
    }
    else {

        switch (par_e_Mode) {

        case GL_TEXTURE_2D:
        {
            /* If GL_TEXTURE_2D is not already enabled, enable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d != SGL_TRUE) {
#ifndef ES2_DEV_ENV
                glEnable(GL_TEXTURE_2D);
#endif
                glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
        }
            break;

        case GL_STENCIL_TEST:
        {
            /* If GL_STENCIL_TEST is not already enabled, enable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test != SGL_TRUE) {
                glEnable(GL_STENCIL_TEST);
                glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
        }
            break;
        case GL_SCISSOR_TEST:
        {
            /* If GL_SCISSOR_TEST is not already enabled, enable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test != SGL_TRUE) {
                glEnable(GL_SCISSOR_TEST);
                glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }

        }
            break;
        case GL_DEPTH_TEST:
        {
            /* If GL_DEPTH_TEST is not already enabled, enable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test != SGL_TRUE) {
                glEnable(GL_DEPTH_TEST);
                glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test = SGL_TRUE;
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
            /* If GL_BLEND is not already enabled, enable it */
            if (glob_pr_sglStatemachine->r_ogl_state.b_state_blend != SGL_TRUE) {
                glEnable(GL_BLEND);
                glob_pr_sglStatemachine->r_ogl_state.b_state_blend = SGL_TRUE;
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
