/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCompressedTexImage2D.c
 DESCRIPTION       : sglCompressedTexImage2D command shall specify a texture 
					from a compressed format
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
#pragma attol crc_justify_all (Rule M17.4, 2000, "Rule M17.4 is not applied here as we do not know by advance the dimensions of the input texture.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxEnlargeTexture
  DESCRIPTION:
    Function shall enlarge texture to a power of two texture
  SCOPE: global
  PARAMETERS:
    par_l_width -> original width of texture in range [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> original height of texture in range [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_l_enlarged_width -> enlarged width of texture in range [SGL_TEXTURE_MIN_WIDTH,glob_pr_sglStatemachine->ul_max_texture_width], it is a power of two
    par_l_enlarged_height -> enlarged height of texture in range [SGL_TEXTURE_MIN_HEIGHT,glob_pr_sglStatemachine->ul_max_texture_height], it is a power of two
    par_l_component_number -> number of color components (3 or 4)
  RETURN:	 
    None  
---------------------------------------------------------------------+*/
void oglxEnlargeTexture(SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_enlarged_width, SGLlong par_l_enlarged_height, SGLlong par_l_component_number)
{
    SGLlong loc_l_line_index_dest;
    SGLlong loc_l_line_index_source;
    SGLlong loc_l_component_index_dest;
    SGLlong loc_l_component_index_source;

    SGLlong loc_l_i;
    SGLlong loc_l_j;
    SGLlong loc_l_k;

    SGLlong loc_l_index_source;
    SGLlong loc_l_index_dest;

    SGLfloat loc_f_u_step;
    SGLfloat loc_f_v_step;


    /* If original width and height are already powers of two, there is nothing to do */
    if ((par_l_enlarged_width != par_l_width) || (par_l_enlarged_height != par_l_height)) {
        loc_f_u_step = SGLfloat_div((SGLfloat) par_l_width, (SGLfloat) par_l_enlarged_width);
        loc_f_v_step = SGLfloat_div((SGLfloat) par_l_height, (SGLfloat) par_l_enlarged_height);

        /* Read the texture from last line to first line */
        for (loc_l_i = (par_l_enlarged_height - 1); loc_l_i >= 0; loc_l_i--) {
            SGLlong loc_l_line_index;
            SGLfloat loc_f_index;
            /* Compute the line index of destination */
            loc_l_line_index_dest = loc_l_i * par_l_enlarged_width;
            loc_f_index = (SGLfloat) loc_l_i;
            loc_f_index = loc_f_v_step * loc_f_index;
            loc_l_line_index = (SGLlong) loc_f_index;
            loc_l_line_index_source = loc_l_line_index * par_l_width;

            /* Read the texture from last column to first column */
            for (loc_l_j = (par_l_enlarged_width - 1); loc_l_j >= 0; loc_l_j--) {
                loc_l_index_dest = par_l_component_number * (loc_l_line_index_dest + loc_l_j);
                loc_f_index = (SGLfloat) loc_l_j;
                loc_f_index = loc_f_u_step * loc_f_index;
                loc_l_line_index = (SGLlong) loc_f_index;

                loc_l_index_source = par_l_component_number * (loc_l_line_index + loc_l_line_index_source);

                /* Copy each color component to the destination texture */
                for (loc_l_k = 0; loc_l_k < par_l_component_number; loc_l_k++) {
                    loc_l_component_index_dest = loc_l_index_dest + loc_l_k;
                    loc_l_component_index_source = loc_l_index_source + loc_l_k;

                    glob_pr_sglStatemachine->tub_texture_buffer[loc_l_component_index_dest] =
                        (GLubyte) (glob_pr_sglStatemachine->tub_texture_buffer[loc_l_component_index_source]);
                }
            }

        }
    }
    else {
        /* Nothing to do */
    }

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxEnlargeTextureKeepDimensions
  DESCRIPTION:
    Function shall enlarge texture to a power of two texture
  SCOPE: global
  PARAMETERS:
    par_l_width -> original width of texture in range [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> original height of texture in range [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_l_enlarged_width -> enlarged width of texture in range [SGL_TEXTURE_MIN_WIDTH,glob_pr_sglStatemachine->ul_max_texture_width], it is a power of two
    par_l_enlarged_height -> enlarged height of texture in range [SGL_TEXTURE_MIN_HEIGHT,glob_pr_sglStatemachine->ul_max_texture_height], it is a power of two
    par_l_component_number -> number of color components (3 or 4)
  RETURN:	 
    None  
---------------------------------------------------------------------+*/
void oglxEnlargeTextureKeepDimensions(SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_enlarged_width, SGLlong par_l_enlarged_height,
                                      SGLlong par_l_component_number)
{
    SGLlong loc_l_line_index_dest;
    SGLlong loc_l_line_index_source;
    SGLlong loc_l_component_index_dest;
    SGLlong loc_l_component_index_source;

    SGLlong loc_l_i;
    SGLlong loc_l_j;
    SGLlong loc_l_k;

    SGLlong loc_l_index_source;
    SGLlong loc_l_index_dest;

    /* If original width and height are already powers of two, there is nothing to do */
    if ((par_l_enlarged_width != par_l_width) || (par_l_enlarged_height != par_l_height)) {

        /* Read the texture from last line to first line */
        for (loc_l_i = (par_l_enlarged_height - 1); loc_l_i >= 0; loc_l_i--) {
            /* Compute the line index of destination */
            loc_l_line_index_dest = loc_l_i * par_l_enlarged_width;
            loc_l_line_index_source = loc_l_i * par_l_width;

            /* Read the texture from last column to first column */
            for (loc_l_j = (par_l_enlarged_width - 1); loc_l_j >= 0; loc_l_j--) {
                loc_l_index_dest = par_l_component_number * (loc_l_line_index_dest + loc_l_j);

                loc_l_index_source = par_l_component_number * (loc_l_j + loc_l_line_index_source);

                if ((loc_l_i < par_l_height) && (loc_l_j < par_l_width)) {
                    /* Copy each color component to the destination texture */
                    for (loc_l_k = 0; loc_l_k < par_l_component_number; loc_l_k++) {
                        loc_l_component_index_dest = loc_l_index_dest + loc_l_k;
                        loc_l_component_index_source = loc_l_index_source + loc_l_k;

                        glob_pr_sglStatemachine->tub_texture_buffer[loc_l_component_index_dest] =
                            (GLubyte) (glob_pr_sglStatemachine->tub_texture_buffer[loc_l_component_index_source]);
                    }
                }
                else {
                    /* Fill with a transparent pixel */
                    for (loc_l_k = 0; loc_l_k < par_l_component_number; loc_l_k++) {
                        loc_l_component_index_dest = loc_l_index_dest + loc_l_k;
                        loc_l_component_index_source = loc_l_index_source + loc_l_k;

                        glob_pr_sglStatemachine->tub_texture_buffer[loc_l_component_index_dest] = 0x0U;
                    }

                }
            }

        }
    }
    else {
        /* Nothing to do */
    }

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  oglxUncompressBitmap
  DESCRIPTION:
    Function shall uncompress a bitmap in glob_pr_sglStatemachine->tub_texture_buffer
  SCOPE: global
  PARAMETERS:
    par_b_format -> bitmap format (SGL_BITMAP_RGB_TRANSPARENT or SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_ALPHA)
    par_l_width -> bitmap width, range: [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> bitmap height, range: [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_l_pixel_array_size -> pixel array size, range: [1, glob_pr_sglStatemachine->ul_max_texture_width*glob_pr_sglStatemachine->ul_max_texture_height]
    par_pul_pixel -> pixel array
  RETURN:	 
    SGLbyte -> SGL_TRUE if uncompress is well performed else SGL_FALSE  
---------------------------------------------------------------------+*/
SGLbyte oglxUncompressBitmap(SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_pixel_array_size, const SGLulong * par_pul_pixel)
{
    SGLbyte loc_ub_result;

    SGLlong loc_l_component_number;
    SGLlong loc_l_index;
    SGLlong loc_l_index_max;
    const SGLulong *loc_pul_pixel;
    const SGLulong *loc_pul_pixel_max;

    if (par_b_format == SGL_BITMAP_RGB_TRANSPARENT) {
        loc_l_component_number = 4;
    }
    else {
        if (par_b_format == SGL_BITMAP_ALPHA) {
            loc_l_component_number = 1;
        }
        else {
            loc_l_component_number = 3;
        }
    }

    loc_pul_pixel = par_pul_pixel;
    loc_pul_pixel_max = par_pul_pixel + par_l_pixel_array_size;
    loc_l_index = 0;
    if (par_b_format == SGL_BITMAP_ALPHA) {
        loc_l_index_max = par_l_width * par_l_height;
    }
    else {
        loc_l_index_max = loc_l_component_number * par_l_width * par_l_height;
    }

    /* Traverse pixel array */
    while (loc_pul_pixel < loc_pul_pixel_max) {

        SGLulong loc_ul_pixel = *loc_pul_pixel;
        GLubyte loc_ub_red, loc_ub_green, loc_ub_blue, loc_ub_alpha;
        SGLulong loc_ul_repeat = 1UL;

        /* Decode pixel components */
        if (loc_l_component_number == 1) {
            loc_ub_red = 0U;
            loc_ub_green = 0U;
            loc_ub_blue = 0U;
        }
        else {
            loc_ub_red = (GLubyte) ((loc_ul_pixel & 0x00FF0000U) >> 16U);
            loc_ub_green = (GLubyte) ((loc_ul_pixel & 0x0000FF00U) >> 8U);
            loc_ub_blue = (GLubyte) ((loc_ul_pixel & 0x000000FFU));
        }

        if (loc_l_component_number == 4) {
            loc_ub_alpha = (GLubyte) ((loc_ul_pixel & 0xFF000000U) >> 24U);
        }
        else {
            if (loc_l_component_number == 1) {
                loc_ub_alpha = (GLubyte) loc_ul_pixel;
            }
            else {
                loc_ub_alpha = (GLubyte) 0xFFU;
            }
        }

        /* If the next pixel is the same, get the repeat value */
        loc_pul_pixel++;
        if ((loc_pul_pixel < loc_pul_pixel_max) && (loc_ul_pixel == *loc_pul_pixel)) {
            loc_pul_pixel++;
            loc_ul_repeat++;
            if (loc_pul_pixel < loc_pul_pixel_max) {
                loc_ul_repeat += *loc_pul_pixel;
                loc_pul_pixel++;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }

        /* Fill the global buffer */
        while (loc_ul_repeat > 0UL) {
            /* Fill texture buffer while texture size is in expected range */
            if ((loc_l_index + loc_l_component_number) <= loc_l_index_max) {
                if (loc_l_component_number > 1) {
                    glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index] = loc_ub_red;
                    loc_l_index++;
                    glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index] = loc_ub_green;
                    loc_l_index++;
                    glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index] = loc_ub_blue;
                    loc_l_index++;
                }
                else {
                    /* Nothing to do */
                }

                if ((loc_l_component_number == 4) || (loc_l_component_number == 1)) {
                    glob_pr_sglStatemachine->tub_texture_buffer[loc_l_index] = loc_ub_alpha;
                    loc_l_index++;
                }
                else {
                    /* Nothing to do */
                }
            }
            else {
                /* The given texture is larger than expected */
                loc_l_index++;
            }
            loc_ul_repeat--;
        }
    }

    /* Set the return status */
    if (loc_l_index == loc_l_index_max) {
        loc_ub_result = SGL_TRUE;
    }
    else {
        loc_ub_result = SGL_FALSE;
    }

    return loc_ub_result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_tex_image_2D_ubv_uncompressed
  DESCRIPTION:
    Function shall load an uncompressed texture in OpenGL driver.
  SCOPE: global
  PARAMETERS:		
    par_l_texture_number -> texture number, range: [0,glob_pr_sglStatemachine->ul_number_of_textures]
    par_b_format -> texture format (SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_RGB_TRANSPARENT, or SGL_BITMAP_ALPHA)
    par_l_width -> texture width, range: [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> texture height, range: [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_b_texture_application_mode -> Application mode (SGL_CLAMP or SGL_REPEAT)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_tex_image_2D_ubv_uncompressed(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height,
                                       SGLbyte par_b_texture_application_mode)
{
    GLenum loc_e_format;
    GLenum loc_e_intern_format;


    switch (par_b_format) {
    case SGL_BITMAP_RGB_TRANSPARENT:
    {
        loc_e_format = (GLenum) GL_RGBA;
        loc_e_intern_format = (GLenum) (glob_pr_sglStatemachine->i_alpha_texture_internal_format);
    }
        break;
    case SGL_BITMAP_ALPHA:
    {
        loc_e_format = (GLenum) GL_ALPHA;
        loc_e_intern_format = (GLenum) GL_ALPHA;
    }
        break;
    default:
        /* case SGL_BITMAP_RGB_NOT_TRANSPARENT: */
    {
        loc_e_format = (GLenum) GL_RGB;
        loc_e_intern_format = (GLenum) (glob_pr_sglStatemachine->i_texture_internal_format);
    }
        break;
    }


    /* Request for a new texture id */
    if (glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ui_texture == 0U) {
        GLuint loc_ui_texture;
        glGenTextures(1, &loc_ui_texture);
        glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ui_texture = (SGLulong) loc_ui_texture;
    }
    else {
        /* Nothing to do */
    }
    glBindTexture(GL_TEXTURE_2D, glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].ui_texture);

    /* Set texture parameters for glTexImage2D */
    sgl_set_texture_parameters(par_b_texture_application_mode);

    /* Load texture in graphic memory */
    glTexImage2D(GL_TEXTURE_2D, (GLint) 0, (GLint) loc_e_intern_format, (GLsizei) par_l_width, (GLsizei) par_l_height, (GLint) 0, loc_e_format,
                 GL_UNSIGNED_BYTE, (GLubyte *) & (glob_pr_sglStatemachine->tub_texture_buffer[0]));

    glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_specified_state = SGL_TRUE;
    glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_format = par_b_format;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_comp_teximage2d_detect_error
  DESCRIPTION:
    Function shall check sglCompressedTexImage2D parameters
  SCOPE: global
  PARAMETERS:
    par_l_texture_number -> texture number
    par_b_format -> texture format(SGL_BITMAP_RGB_TRANSPARENT or SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_ALPHA)
    par_l_width -> texture width, range: [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> texture height, range: [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_l_pixel_array_size -> pixel array size, range: [1, glob_pr_sglStatemachine->ul_max_texture_width*glob_pr_sglStatemachine->ul_max_texture_height]
    par_pul_pixel -> pixel array
    par_b_texture_application_mode -> Application mode (SGL_CLAMP or SGL_REPEAT)
  RETURN:
    SGLbyte -> Error detected (1U) or not (0U)
---------------------------------------------------------------------+*/
SGLbyte sgl_comp_teximage2d_detect_error(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height,
                                         const SGLulong * par_pul_pixel, SGLbyte par_b_texture_application_mode)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check texture number is not negative */
    if (par_l_texture_number < 0L) {
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

    /* Check Function is not called between sglBegin/sglEnd */
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

    /* Check texture attribute buffer is not NULL */
    if (glob_pr_sglStatemachine->p_texture_attrib == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check this texture is not already bound */
    if (glob_pr_sglStatemachine->b_texture_state && (par_l_texture_number == glob_pr_sglStatemachine->l_last_texture_bound_index)) {
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

    /* Check texture number is valid */
    if (par_l_texture_number >= (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check format is SGL_BITMAP_RGB_TRANSPARENT or SGL_BITMAP_RGB_NOT_TRANSPARENT */
    if ((par_b_format != SGL_BITMAP_RGB_TRANSPARENT) && (par_b_format != SGL_BITMAP_RGB_NOT_TRANSPARENT)
        && (par_b_format != SGL_BITMAP_ALPHA)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture is not NULL */
    if (par_pul_pixel == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglCompressedTexImage2D
  DESCRIPTION:
    Function shall specify a texture from a compressed format.
  SCOPE: global
  PARAMETERS:
    par_l_texture_number -> texture number
    par_b_format -> texture format(SGL_BITMAP_RGB_TRANSPARENT or SGL_BITMAP_RGB_NOT_TRANSPARENT or SGL_BITMAP_ALPHA)
    par_l_width -> texture width, range: [1,glob_pr_sglStatemachine->ul_max_texture_width]
    par_l_height -> texture height, range: [1,glob_pr_sglStatemachine->ul_max_texture_height]
    par_l_pixel_array_size -> pixel array size, range: [1, glob_pr_sglStatemachine->ul_max_texture_width*glob_pr_sglStatemachine->ul_max_texture_height]
    par_pul_pixel -> pixel array
    par_b_texture_application_mode -> Application mode (SGL_CLAMP or SGL_REPEAT)
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglCompressedTexImage2D(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_pixel_array_size,
                             const SGLulong * par_pul_pixel, SGLbyte par_b_texture_application_mode)
{
    SGLbyte loc_ub_uncompressed_buffer;

    SGLbyte loc_b_error_detected = sgl_comp_teximage2d_detect_error(par_l_texture_number, par_b_format, par_l_width, par_l_height, par_pul_pixel,
                                                                    par_b_texture_application_mode);
    if (!loc_b_error_detected) {

        /* Uncompress the data */
        loc_ub_uncompressed_buffer = oglxUncompressBitmap(par_b_format, par_l_width, par_l_height, par_l_pixel_array_size, par_pul_pixel);

        /* Do the real job */
        if (loc_ub_uncompressed_buffer) {
            SGLlong loc_l_width = mth_next_higher_power2(par_l_width, (SGLlong) SGL_TEXTURE_MIN_WIDTH, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width);
            SGLlong loc_l_height =
                mth_next_higher_power2(par_l_height, (SGLlong) SGL_TEXTURE_MIN_HEIGHT, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height);
            SGLlong loc_l_component_number;

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

            if (par_b_format == SGL_BITMAP_RGB_TRANSPARENT) {
                loc_l_component_number = 4;
            }
            else {
                if (par_b_format == SGL_BITMAP_ALPHA) {
                    loc_l_component_number = 1;
                }
                else {
                    loc_l_component_number = 3;
                }
            }

            if (par_b_texture_application_mode == SGL_CLAMP) {
                /* Enlarge texture to the next power of two */
                oglxEnlargeTextureKeepDimensions(par_l_width, par_l_height, loc_l_width, loc_l_height, loc_l_component_number);
            }
            else {
                /* Enlarge texture to the next power of two */
                oglxEnlargeTexture(par_l_width, par_l_height, loc_l_width, loc_l_height, loc_l_component_number);
            }

            sgl_tex_image_2D_ubv_uncompressed(par_l_texture_number, par_b_format, loc_l_width, loc_l_height, par_b_texture_application_mode);

            /* Reset bound texture in case this function is called after a draw */
            glob_pr_sglStatemachine->ui_ogl_texture_bound = NO_OGL_TEXTURE_BOUND;
        }
        else {
            oglxSetError(SGL_ERROR_SGL_COMPRESSEDTEXIMAGE2D, (SGLulong) par_l_texture_number);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_COMPRESSEDTEXIMAGE2D, (SGLulong) par_l_texture_number);
    }
    return;
}

/* End of File ***************************************************************/
