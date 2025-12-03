/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluDrawBitmap.c
 DESCRIPTION       : sgluDrawBitmap command shall draw given bitmap
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
#include "mth.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluDrawBitmap
  DESCRIPTION:
    Function shall draw given bitmap.
  SCOPE: global
  PARAMETERS:
    par_l_texture_number -> Texture id of the bitmap
    par_f_originx -> X origin of the bitmap
    par_f_originy -> Y origin of the bitmap
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgluDrawBitmap(SGLlong par_l_texture_number, SGLfloat par_f_originx, SGLfloat par_f_originy)
{
    /* Check that the texture ID is one that exists in the texture table */
    if ((par_l_texture_number < 0L) || (par_l_texture_number >= (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures)
        || (glob_pr_sglStatemachine->p_texture_attrib == SGL_NULL)) {
        oglxSetError(SGL_ERROR_SGLU_DRAWBITMAP, (SGLulong) par_l_texture_number);
    }
    else {
        if ((glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_specified_state != SGL_TRUE)
            || (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX)) {
            oglxSetError(SGL_ERROR_SGLU_DRAWBITMAP, (SGLulong) par_l_texture_number);
        }
        else {
            SGLfloat loc_f_texture_width = (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_textures_dimension[0];
            SGLfloat loc_f_texture_height = (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_textures_dimension[1];

            SGLfloat loc_f_x0 = par_f_originx;
            SGLfloat loc_f_y0 = par_f_originy;
            SGLfloat loc_f_x1 = loc_f_x0 + (loc_f_texture_width * glob_pr_sglStatemachine->f_ratio_scale_width);
            SGLfloat loc_f_y1 = loc_f_y0 + (loc_f_texture_height * glob_pr_sglStatemachine->f_ratio_scale_height);

            SGLbyte loc_b_previous_texture_state = glob_pr_sglStatemachine->b_texture_state;
            SGLbyte loc_b_previous_modulate = glob_pr_sglStatemachine->b_modulate;

            SGLfloat loc_f_u_factor =
                SGLfloat_div(loc_f_texture_width, (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_dimension_power_2[0]);
            SGLfloat loc_f_v_factor =
                SGLfloat_div(loc_f_texture_height, (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_dimension_power_2[1]);

            if (loc_b_previous_modulate || glob_pr_sglStatemachine->b_static_sequence_started) {
                glob_pr_sglStatemachine->b_modulate = SGL_FALSE;
#ifndef ES2_DEV_ENV
                oglxTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
#endif
            }
            else {
                /* Nothing to do */
            }

            sglBindTexture(SGL_TEXTURE_2D, (SGLlong) par_l_texture_number);
            {
                SGLfloat lX[2];
                SGLfloat lY[2];

                lX[0] = loc_f_x0;
                lY[0] = loc_f_y0;
                lX[1] = loc_f_x1;
                lY[1] = loc_f_y1;

                sglFormatTexture(2, lX, lY, 1.0F, 1.0F, SGL_ALIGN_LEFT, SGL_ALIGN_BOTTOM);
            }

            sglEnable(SGL_TEXTURE_2D);

            if (glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_textures_app_mode == SGL_CLAMP) {
                sglBegin(SGL_POLYGON);
                oglxTexVertex4f(loc_f_x0, loc_f_y0, 0.0F, 0.0F);
                oglxTexVertex4f(loc_f_x1, loc_f_y0, loc_f_u_factor, 0.0F);
                oglxTexVertex4f(loc_f_x1, loc_f_y1, loc_f_u_factor, loc_f_v_factor);
                oglxTexVertex4f(loc_f_x0, loc_f_y1, 0.0F, loc_f_v_factor);
                sglEnd();
            }
            else {
                sglBegin(SGL_POLYGON);
                oglxVertex2f(loc_f_x0, loc_f_y0);
                oglxVertex2f(loc_f_x1, loc_f_y0);
                oglxVertex2f(loc_f_x1, loc_f_y1);
                oglxVertex2f(loc_f_x0, loc_f_y1);
                sglEnd();
            }


            /* Restore previous texture state if necessary */
            if (loc_b_previous_texture_state != SGL_TRUE) {
                sglDisable(SGL_TEXTURE_2D);
            }
            else {
                /* Nothing to do */
            }

            if (loc_b_previous_modulate) {
                glob_pr_sglStatemachine->b_modulate = SGL_TRUE;
#ifndef ES2_DEV_ENV
                oglxTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_MODULATE);
#endif
            }
            else {
                /* Nothing to do */
            }

        }
    }
    return;
}
