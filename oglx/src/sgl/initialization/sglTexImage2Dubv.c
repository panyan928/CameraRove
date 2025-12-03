/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglTexImage2Dubv.c
 DESCRIPTION       : sglTexImage2Dubv shall specify a texture
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
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_set_texture_parameters
  DESCRIPTION:
    Function shall set the texture parameters
  SCOPE: global
  PARAMETERS:		
    par_b_texture_application_mode -> Application mode (SGL_CLAMP or SGL_REPEAT)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_texture_parameters(SGLbyte par_b_texture_application_mode)
{
    /* Set texture parameters for glTexImage2D */
#ifndef ES2_DEV_ENV
    oglxTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
#endif

    if (par_b_texture_application_mode == SGL_CLAMP) {
#if defined(ES2_DEV_ENV) || defined(SC101_DEV_ENV) || defined(ES11_DEV_ENV)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_CLAMP_TO_EDGE);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_CLAMP);
#endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) GL_NEAREST);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) GL_LINEAR);
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_teximage_2dubv_detect_error
  DESCRIPTION:
    Function shall check sglTexImage2Dubv parameters
  SCOPE: global
  PARAMETERS:		
    par_l_texture_number -> texture number, range: [0,glob_pr_sglStatemachine->ul_number_of_textures]
    par_b_format -> texture format (SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_RGB_TRANSPARENT or SGL_BITMAP_ALPHA)
    par_l_width -> texture width, range: [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> texture height, range: [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_pb_color -> texture description
    par_b_texture_application_mode -> Application mode (SGL_CLAMP or SGL_REPEAT)
  RETURN:
    SGLbyte -> Error detected (1U) or not (0U)
---------------------------------------------------------------------+*/
SGLbyte sgl_teximage_2dubv_detect_error(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height,
                                        const SGLbyte * par_pb_color, SGLbyte par_b_texture_application_mode)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check color buffer is not NULL */
    if (par_pb_color == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture number is not negative */
    if (par_l_texture_number < 0L) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check last texture bound is not the same as this one */
    if (glob_pr_sglStatemachine->b_texture_state && (par_l_texture_number == glob_pr_sglStatemachine->l_last_texture_bound_index)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture buffer is not NULL */
    if (glob_pr_sglStatemachine->tub_texture_buffer == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture application mode is SGL_CLAMP or SGL_REPEAT */
    if ((par_b_texture_application_mode != SGL_CLAMP) && (par_b_texture_application_mode != SGL_REPEAT)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }
    /* Check texture attribute buffer is not NULL */
    if (glob_pr_sglStatemachine->p_texture_attrib == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture number is valid */
    if (par_l_texture_number >= (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check width is positive */
    if (par_l_width <= 0L) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check width is valid */
    if (par_l_width > (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check height is positive */
    if (par_l_height <= 0L) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check height is valid */
    if (par_l_height > (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check format is equal to SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_RGB_TRANSPARENT */
    if ((par_b_format != SGL_BITMAP_RGB_TRANSPARENT) && (par_b_format != SGL_BITMAP_RGB_NOT_TRANSPARENT)
        && (par_b_format != SGL_BITMAP_ALPHA)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglTexImage2Dubv
  DESCRIPTION:
    Function shall specify a texture.
  SCOPE: global
  PARAMETERS:		
    par_l_texture_number -> texture number, range: [0,glob_pr_sglStatemachine->ul_number_of_textures]
    par_b_format -> texture format (SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_RGB_TRANSPARENT or SGL_BITMAP_ALPHA)
    par_l_width -> texture width, range: [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> texture height, range: [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_pb_color -> texture description
    par_b_texture_application_mode -> Application mode (SGL_CLAMP or SGL_REPEAT)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglTexImage2Dubv(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height, const SGLbyte * par_pb_color,
                      SGLbyte par_b_texture_application_mode)
{
    SGLlong loc_l_line_index_dest;
    SGLlong loc_l_line_index_source;

    SGLlong loc_l_i;
    SGLlong loc_l_j;
    SGLlong loc_l_k;

    SGLlong loc_l_index_source;
    SGLlong loc_l_index_dest;

    SGLfloat loc_f_u_step;
    SGLfloat loc_f_v_step;

    SGLlong loc_l_width;
    SGLlong loc_l_height;

    SGLlong loc_l_component_number;
    GLenum loc_e_format;
    GLenum loc_e_intern_format;

    SGLbyte loc_b_error_detected =
        sgl_teximage_2dubv_detect_error(par_l_texture_number, par_b_format, par_l_width, par_l_height, par_pb_color, par_b_texture_application_mode);
    if (!loc_b_error_detected) {

        /* Set current texture format */
        switch (par_b_format) {
        case SGL_BITMAP_RGB_TRANSPARENT:
        {
            loc_l_component_number = 4;
            loc_e_format = (GLenum) GL_RGBA;
            loc_e_intern_format = (GLenum) (glob_pr_sglStatemachine->i_alpha_texture_internal_format);
        }
            break;
        case SGL_BITMAP_ALPHA:
        {
            loc_l_component_number = 1;
            loc_e_format = (GLenum) GL_ALPHA;
            loc_e_intern_format = (GLenum) GL_ALPHA;
        }
            break;
        default:
            /*
               case SGL_BITMAP_RGB_NOT_TRANSPARENT:
             */
        {
            loc_l_component_number = 3;
            loc_e_format = (GLenum) GL_RGB;
            loc_e_intern_format = (GLenum) (glob_pr_sglStatemachine->i_texture_internal_format);
        }
            break;
        }

        /* Require a new texture id */
        if (glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ui_texture == 0U) {
            GLuint loc_ui_texture;
            glGenTextures(1, &loc_ui_texture);
            glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ui_texture = (SGLulong) loc_ui_texture;
        }
        else {
            /* Nothing to do */
        }
        glBindTexture(GL_TEXTURE_2D, glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ui_texture);

        /* Compute width and height of enlarged texture */
        loc_l_width = mth_next_higher_power2(par_l_width, (SGLlong) SGL_TEXTURE_MIN_WIDTH, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width);
        loc_l_height = mth_next_higher_power2(par_l_height, (SGLlong) SGL_TEXTURE_MIN_HEIGHT, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height);

        if ((loc_l_width > loc_l_height) && (loc_l_width <= (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height)) {
            loc_l_height = loc_l_width;
        }
        else {
            if (loc_l_height <= (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width) {
                loc_l_width = loc_l_height;
            }
        }

        /* Store texture dimensions */
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_textures_dimension[0] = (SGLulong) par_l_width;
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_textures_dimension[1] = (SGLulong) par_l_height;
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_dimension_power_2[0] = loc_l_width;
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ul_dimension_power_2[1] = loc_l_height;
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_textures_app_mode = par_b_texture_application_mode;

        loc_f_u_step = SGLfloat_div((SGLfloat) par_l_width, (SGLfloat) loc_l_width);
        loc_f_v_step = SGLfloat_div((SGLfloat) par_l_height, (SGLfloat) loc_l_height);

        if (par_b_texture_application_mode == SGL_CLAMP) {
            /* Fill global texture buffer with required texture */
            for (loc_l_i = 0; loc_l_i < loc_l_height; loc_l_i++) {
                SGLlong loc_l_line_index;
                SGLfloat loc_f_index;

                loc_l_line_index_dest = loc_l_i * loc_l_width;

                loc_f_index = (SGLfloat) loc_l_i;
                loc_l_line_index = (SGLlong) loc_f_index;
                loc_l_line_index_source = loc_l_line_index * par_l_width;

                for (loc_l_j = 0; loc_l_j < loc_l_width; loc_l_j++) {
                    loc_l_index_dest = loc_l_component_number * (loc_l_line_index_dest + loc_l_j);

                    loc_f_index = (SGLfloat) loc_l_j;
                    loc_l_line_index = (SGLlong) loc_f_index;
                    loc_l_index_source = loc_l_component_number * (loc_l_line_index + loc_l_line_index_source);

                    if ((loc_l_i < par_l_height) && (loc_l_j < par_l_width)) {
                        for (loc_l_k = 0; loc_l_k < loc_l_component_number; loc_l_k++) {
                            glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index_dest + loc_l_k] = (GLubyte) (par_pb_color[loc_l_index_source + loc_l_k]);
                        }
                    }
                    else {
                        for (loc_l_k = 0; loc_l_k < loc_l_component_number; loc_l_k++) {
                            glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index_dest + loc_l_k] = 0x0U;
                        }
                    }
                }
            }

        }
        else {
            /* Fill global texture buffer with required texture */
            for (loc_l_i = 0; loc_l_i < loc_l_height; loc_l_i++) {
                SGLlong loc_l_line_index;
                SGLfloat loc_f_index;

                loc_l_line_index_dest = loc_l_i * loc_l_width;

                loc_f_index = (SGLfloat) loc_l_i;
                loc_f_index = loc_f_v_step * loc_f_index;
                loc_l_line_index = (SGLlong) loc_f_index;
                loc_l_line_index_source = loc_l_line_index * par_l_width;

                for (loc_l_j = 0; loc_l_j < loc_l_width; loc_l_j++) {
                    loc_l_index_dest = loc_l_component_number * (loc_l_line_index_dest + loc_l_j);

                    loc_f_index = (SGLfloat) loc_l_j;
                    loc_f_index = loc_f_u_step * loc_f_index;
                    loc_l_line_index = (SGLlong) loc_f_index;
                    loc_l_index_source = loc_l_component_number * (loc_l_line_index + loc_l_line_index_source);

                    for (loc_l_k = 0; loc_l_k < loc_l_component_number; loc_l_k++) {
                        glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index_dest + loc_l_k] = (GLubyte) (par_pb_color[loc_l_index_source + loc_l_k]);
                    }
                }
            }
        }

        sgl_set_texture_parameters(par_b_texture_application_mode);

        /* Load texture in graphic memory */
        glTexImage2D(GL_TEXTURE_2D, (GLint) 0, (GLint) loc_e_intern_format, (GLsizei) loc_l_width, (GLsizei) loc_l_height, (GLint) 0, loc_e_format,
                     GL_UNSIGNED_BYTE, (GLubyte *) & (glob_pr_sglStatemachine->tub_texture_buffer[0]));

        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_specified_state = SGL_TRUE;
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_format = par_b_format;

        /* Reset bound texture in case this function is called after a draw */
        glob_pr_sglStatemachine->ui_ogl_texture_bound = NO_OGL_TEXTURE_BOUND;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_TEXIMAGE2DUBV, (SGLulong) par_l_texture_number);
    }
    return;
}

/* End of File ***************************************************************/
