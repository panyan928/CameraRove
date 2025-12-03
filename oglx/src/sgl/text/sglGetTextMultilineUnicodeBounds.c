/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetTextMultilineUnicodeBounds.c
 DESCRIPTION       : sglGetTextMultilineUnicodeBounds command shall return the bounds of a multiline text.
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
  NAME: sglGetTextMultilineUnicodeBounds
  DESCRIPTION:
    Function shall return the bounds of a multiline text.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pl_string -> string to be drawn
    par_l_string_length -> string length
    par_l_font_id -> font to be taken into account
	par_l_hor_alignment -> horizontal alignment
	par_l_vert_alignment -> vertical alignment
  RETURN:
    None
---------------------------------------------------------------------+*/
sgl_bounding_box sglGetTextMultilineUnicodeBounds(SGLfloat par_f_x, SGLfloat par_f_y,
                                                  const SGLlong * par_pl_string, SGLlong par_l_string_length,
                                                  SGLlong par_l_font_id, SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    SGLulong loc_ul_i = 0UL;
    sgl_bounding_box loc_s_return;
    SGLbyte loc_b_error_detected = 0U;

    loc_s_return.f_min_x = 0.0F;
    loc_s_return.f_max_x = 0.0F;
    loc_s_return.f_min_y = 0.0F;
    loc_s_return.f_max_y = 0.0F;

    /* Check the string pointer */
    if ((par_pl_string == SGL_NULL) || (par_l_string_length < 0L) || (par_l_font_id < 0L)) {
        loc_b_error_detected = 1U;
    }
    else {
        while ((loc_ul_i < (SGLulong) par_l_string_length) && (par_pl_string[loc_ul_i] != 0L) && (!loc_b_error_detected)) {
            if (par_pl_string[loc_ul_i] < 0) {
                loc_b_error_detected = 1U;
            }
            else {
                /* Nothing to do */
            }
            loc_ul_i++;
        }
    }

    /* Check horizontal alignment */
    if ((par_l_hor_alignment != SGL_ALIGN_LEFT) && (par_l_hor_alignment != SGL_ALIGN_CENTER) && (par_l_hor_alignment != SGL_ALIGN_RIGHT)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check vertical alignment */
    if ((par_l_vert_alignment != SGL_ALIGN_BOTTOM) && (par_l_vert_alignment != SGL_ALIGN_MIDDLE) && (par_l_vert_alignment != SGL_ALIGN_TOP)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLulong loc_ui_param_length;
        if (par_l_string_length > (SGLlong) SGLU_MAX_STRING_LENGTH) {
            par_l_string_length = (SGLlong) SGLU_MAX_STRING_LENGTH;
        }
        else {
            /* Nothing to do */
        }

        loc_ui_param_length = par_l_string_length;
        par_l_string_length = 0U;

        while (((SGLulong) par_l_string_length < loc_ui_param_length) && (par_pl_string[par_l_string_length] != 0L)) {
            glob_us_string[par_l_string_length] = (GLushort) par_pl_string[par_l_string_length];
            par_l_string_length++;
        }

        glob_us_string[par_l_string_length] = 0;

        loc_s_return = oglxComputeTextBounds(par_f_x, par_f_y, par_l_string_length, par_l_font_id, par_l_hor_alignment, par_l_vert_alignment);
    }
    else {
        loc_s_return.f_min_x = 0.0F;
        loc_s_return.f_min_y = 0.0F;
        loc_s_return.f_max_x = 0.0F;
        loc_s_return.f_max_y = 0.0F;
        loc_s_return.ul_nb_lines = 0UL;
    }

    return loc_s_return;
}

/* End of File ***************************************************************/
