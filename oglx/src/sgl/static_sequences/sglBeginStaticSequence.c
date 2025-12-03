/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBeginStaticSequence.c
 DESCRIPTION       : sglBeginStaticSequence shall begin a sequence of static objects 
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
  NAME: sglBeginStaticSequence
  DESCRIPTION:
    Function shall begin a sequence of static objects 
  SCOPE: global
  PARAMETERS:
    par_f_min_x, par_f_min_y -> lower left corner of the static sequence bounding box relatively to (par_f_origin_x, par_f_origin_y)
	par_f_max_x, par_f_max_y -> upper right corner of the static sequence bounding box relatively to (par_f_origin_x, par_f_origin_y)
  RETURN:
    SGLulong -> the index of the sequence (to use when calling sglCallStaticSequence).
---------------------------------------------------------------------+*/
SGLulong sglBeginStaticSequence(SGLfloat par_f_min_x, SGLfloat par_f_min_y, SGLfloat par_f_max_x, SGLfloat par_f_max_y)
{
    SGLulong loc_ul_return = 0UL;

    if ((par_f_min_x <= par_f_max_x) && (par_f_min_y <= par_f_max_y)) {

#ifdef OGLX_DISPLAY_LISTS
        loc_ul_return = (SGLulong) glGenLists(1);
        glNewList((GLuint) loc_ul_return, GL_COMPILE);

#endif

#ifdef OGLX_FBO
        if (glob_pr_sglStatemachine->ul_static_bitmap_number <= SGL_MAX_STATIC_TEXTURE) {
            GLuint loc_ui_framebuffer;
            GLuint loc_ui_texture;
            GLenum loc_e_status;
            GLint loc_i_texture_width = (GLint) (par_f_max_x - par_f_min_x);
            GLint loc_i_texture_height = (GLint) (par_f_max_y - par_f_min_y);
            SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_static_bitmap_number;

            glGenFramebuffers(1, &loc_ui_framebuffer);
            glGenTextures(1, &loc_ui_texture);

            loc_ul_return = (SGLulong) loc_ui_texture;

            glBindTexture(GL_TEXTURE_2D, loc_ui_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         (SGLulong) (SGLfloat_div
                                     ((SGLfloat) loc_i_texture_width, (glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_width_factor)))
                         ,
                         (SGLulong) (SGLfloat_div
                                     ((SGLfloat) loc_i_texture_height, (glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_height_factor))), 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, SGL_NULL);

            glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_width = loc_i_texture_width;
            glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_height = loc_i_texture_height;
            glob_pr_sglStatemachine->s_static_data[loc_ul_index].f_x = par_f_min_x;
            glob_pr_sglStatemachine->s_static_data[loc_ul_index].f_y = par_f_min_y;
            glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_id = loc_ul_return;
            glob_pr_sglStatemachine->s_static_data[loc_ul_index].b_defined = SGL_TRUE;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) GL_LINEAR);

            glBindFramebuffer(GL_FRAMEBUFFER, loc_ui_framebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, loc_ui_texture, 0);

            loc_e_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

            if (loc_e_status == GL_FRAMEBUFFER_COMPLETE) {
                SGLfloat loc_f_tx = -par_f_min_x;
                SGLfloat loc_f_ty = -par_f_min_y;

                glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
                glClear(GL_COLOR_BUFFER_BIT);
                glBlendColor(0.0F, 0.0F, 0.0F, 0.0F);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


                oglxPushMatrix();

                glob_pr_sglStatemachine->f_modelview_matrix[12] += ((glob_pr_sglStatemachine->f_modelview_matrix[0] * loc_f_tx)
                                                                    + (glob_pr_sglStatemachine->f_modelview_matrix[4] * loc_f_ty));
                glob_pr_sglStatemachine->f_modelview_matrix[13] += ((glob_pr_sglStatemachine->f_modelview_matrix[1] * loc_f_tx)
                                                                    + (glob_pr_sglStatemachine->f_modelview_matrix[5] * loc_f_ty));

                oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);

                glob_pr_sglStatemachine->ul_framebuffer = (SGLulong) loc_ui_framebuffer;

                loc_ul_return = loc_ul_index;
                glob_pr_sglStatemachine->ul_static_bitmap_number = loc_ul_index + 1;
            }
        }
        else {
            /* Error the maximum number of fbo is reached */
        }
#endif
        glob_pr_sglStatemachine->b_static_sequence_started = SGL_TRUE;
    }
    else {
        /* Error the bounding box is not a valid one */
    }

    return loc_ul_return;
}

/* End of File ***************************************************************/
