/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCreateStaticBitmap.c
 DESCRIPTION       : sglCreateStaticBitmap shall create a static bitmap and return an OpenGL texture index
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
#include "mth.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_load_texture
  DESCRIPTION:
    Function shall load a texture and return an OpenGL texture index 
  SCOPE: global
  PARAMETERS:
    par_l_width -> texture width
	par_l_height -> texture height

  RETURN:
    SGLulong -> the index of the texture to draw.
---------------------------------------------------------------------+*/
SGLulong oglx_load_texture(SGLlong par_l_width, SGLlong par_l_height)
{
    GLuint loc_ui_texture;
    glGenTextures(1, &loc_ui_texture);
    glBindTexture(GL_TEXTURE_2D, loc_ui_texture);

    /* Set texture parameters for glTexImage2D */
    sgl_set_texture_parameters(SGL_REPEAT);

    /* Load texture in graphic memory */
    glTexImage2D(GL_TEXTURE_2D, (GLint) 0, GL_RGBA, (GLsizei) par_l_width, (GLsizei) par_l_height, (GLint) 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, (GLubyte *) & (glob_pr_sglStatemachine->tub_texture_buffer[0]));

    return (SGLulong) loc_ui_texture;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglCreateStaticBitmap
  DESCRIPTION:
    Function shall create a static bitmap and return a bitmap index .
  SCOPE: global
  PARAMETERS:
    par_f_min_x, par_f_min_y -> lower left corner of the bitmap
    par_l_width -> texture width
	par_l_height -> texture height
	par_l_pixel_array_size -> size of par_p_pixel
	par_p_pixel -> input texture
	par_b_compressed -> true if input texture is compressed, false if it is not compressed
  RETURN:
    SGLulong -> the index of the texture to draw.
---------------------------------------------------------------------+*/
SGLulong sglCreateStaticBitmap(SGLfloat par_f_min_x, SGLfloat par_f_min_y,
                               SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_pixel_array_size, const void *par_p_pixel, SGLbool par_b_compressed)
{
    SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_static_bitmap_number;
    SGLlong loc_l_width = mth_next_higher_power2(par_l_width, (SGLlong) SGL_TEXTURE_MIN_WIDTH, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width);
    SGLlong loc_l_height = mth_next_higher_power2(par_l_height, (SGLlong) SGL_TEXTURE_MIN_HEIGHT, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height);

    if ((loc_ul_index < SGL_MAX_STATIC_TEXTURE) && (glob_pr_sglStatemachine->tub_texture_buffer != SGL_NULL)
        && (SGLint32_to_uint32(loc_l_width) <= glob_pr_sglStatemachine->ul_max_texture_width)
        && (SGLint32_to_uint32(loc_l_height) <= glob_pr_sglStatemachine->ul_max_texture_height)) {
        SGLulong loc_ul_texture_id = 0UL;
        if (par_b_compressed) {
            const SGLulong *loc_pul_pixel = (const SGLulong *) par_p_pixel;

            SGLbyte loc_b_uncompressed = oglxUncompressBitmap(SGL_BITMAP_RGB_TRANSPARENT, par_l_width, par_l_height, par_l_pixel_array_size, loc_pul_pixel);

            if (loc_b_uncompressed) {
                oglxEnlargeTextureKeepDimensions(par_l_width, par_l_height, loc_l_width, loc_l_height, 4);
                loc_ul_texture_id = oglx_load_texture(loc_l_width, loc_l_height);
            }
            else {
                oglxSetError(SGL_ERROR_SGL_CREATESTATICBITMAP, 1UL);
            }
        }
        else {
            const SGLbyte *loc_pb_pixel = (const SGLbyte *) par_p_pixel;
            SGLulong loc_l_i;
            for (loc_l_i = 0; loc_l_i < (SGLulong) par_l_pixel_array_size; loc_l_i++) {
                glob_pr_sglStatemachine->tub_texture_buffer[loc_l_i] = (GLubyte) (loc_pb_pixel[loc_l_i]);
            }

            oglxEnlargeTextureKeepDimensions(par_l_width, par_l_height, loc_l_width, loc_l_height, 4);
            loc_ul_texture_id = oglx_load_texture(loc_l_width, loc_l_height);
        }
        glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_width = sglInt32ToUInt32(par_l_width);
        glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_height = sglInt32ToUInt32(par_l_height);
        glob_pr_sglStatemachine->s_static_data[loc_ul_index].f_x = par_f_min_x;
        glob_pr_sglStatemachine->s_static_data[loc_ul_index].f_y = par_f_min_y;
        glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_id = loc_ul_texture_id;
        glob_pr_sglStatemachine->s_static_data[loc_ul_index].b_defined = SGL_TRUE;

        glob_pr_sglStatemachine->ul_static_bitmap_number++;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CREATESTATICBITMAP, 0UL);
    }

    return loc_ul_index;
}

/* End of File ***************************************************************/
