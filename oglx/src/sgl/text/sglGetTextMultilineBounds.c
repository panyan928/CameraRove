/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetTextMultilineBounds.c
 DESCRIPTION       : sglGetTextMultilineBounds command shall return the bounds of a multiline text.
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
#pragma attol crc_justify_all (Rule M11.5, 2000, "Rule M11.5 is not applied here as the input string is only read, it is not modified.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxComputeTextBounds
  DESCRIPTION:
    Function shall return the bounds of a multiline text.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_l_string_length -> string length
    par_l_font_id -> font to be taken into account
	par_l_hor_alignment -> horizontal alignment
	par_l_vert_alignment -> vertical alignment
  RETURN:
    None
---------------------------------------------------------------------+*/
sgl_bounding_box oglxComputeTextBounds(SGLfloat par_f_x, SGLfloat par_f_y,
                                       SGLlong par_l_string_length, SGLlong par_l_font_id, SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    SGLfloat loc_f_char_height;
    SGLfloat loc_f_v_advance;
    SGLfloat loc_f_h_advance;
    SGLulong loc_ul_i;
    SGLulong loc_ul_font_id = (SGLulong) par_l_font_id;
    SGLulong loc_ul_number_of_lines = 1UL;
    SGLulong loc_ul_nb_CR = 0U;
    sgl_bounding_box loc_s_return;
    SGLulong loc_ul_max_length = 0UL;

    loc_s_return.f_min_x = 0.0F;
    loc_s_return.f_max_x = 0.0F;
    loc_s_return.f_min_y = 0.0F;
    loc_s_return.f_max_y = 0.0F;

    if (loc_ul_font_id >= glob_pr_sglStatemachine->ul_nb_of_fonts) {
        loc_ul_font_id = INVALID_FONT_ID;
    }
    else {
        /* Nothing to do */
    }

    loc_f_char_height = str_get_string_height(glob_us_string, par_l_string_length, loc_ul_font_id);
    loc_f_v_advance = str_get_string_advance(glob_us_string, par_l_string_length, loc_ul_font_id, SGL_TRUE);
    loc_f_h_advance = str_get_string_advance(glob_us_string, par_l_string_length, loc_ul_font_id, SGL_FALSE);

    oglxSelectFont((SGLulong) par_l_font_id);

    loc_ul_nb_CR = sgl_define_lines(glob_us_string, (SGLulong) par_l_string_length);

    for (loc_ul_i = 0UL; loc_ul_i < (SGLulong) par_l_string_length; loc_ul_i++) {
        if (glob_us_string[loc_ul_i] == SGL_K_LF) {
            loc_ul_number_of_lines++;
        }
        else {
            /* Nothing to do */
        }
    }

    for (loc_ul_i = 0UL; loc_ul_i < loc_ul_nb_CR; loc_ul_i++) {
        SGLulong loc_ul_length = ts_lines[loc_ul_i].ul_substring_length;
        if (loc_ul_length > loc_ul_max_length) {
            SGLulong loc_ul_initial_length = loc_ul_length;
            SGLulong loc_ul_j;
            /* Suppress LF from the number of characters */
            for (loc_ul_j = 0UL; loc_ul_j < loc_ul_initial_length; loc_ul_j++) {
                if (ts_lines[loc_ul_i].pb_substring[loc_ul_j] == SGL_K_LF) {
                    loc_ul_length--;
                }
                else {
                    /* Nothing to do */
                }
            }

            if (loc_ul_length > loc_ul_max_length) {
                loc_ul_max_length = loc_ul_length;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }

    }

    switch (par_l_vert_alignment) {
    case SGL_ALIGN_MIDDLE:
        loc_s_return.f_max_y = par_f_y + ((SGLfloat) loc_ul_number_of_lines * ((loc_f_char_height + loc_f_v_advance) * 0.5F));
        loc_s_return.f_min_y = loc_s_return.f_max_y - ((SGLfloat) loc_ul_number_of_lines * (loc_f_char_height + loc_f_v_advance));
        break;
    case SGL_ALIGN_BOTTOM:
        loc_s_return.f_min_y = par_f_y;
        loc_s_return.f_max_y = loc_s_return.f_min_y + ((SGLfloat) loc_ul_number_of_lines * (loc_f_char_height + loc_f_v_advance));
        break;
    default:
        /*
           case SGL_ALIGN_TOP:
         */
        loc_s_return.f_max_y = par_f_y;
        loc_s_return.f_min_y = loc_s_return.f_max_y - ((SGLfloat) loc_ul_number_of_lines * (loc_f_char_height + loc_f_v_advance));
        break;
    }

    switch (par_l_hor_alignment) {
    case SGL_ALIGN_CENTER:
        loc_s_return.f_min_x = par_f_x - (((SGLfloat) loc_ul_max_length * loc_f_h_advance) * 0.5F);
        loc_s_return.f_max_x = par_f_x + (((SGLfloat) loc_ul_max_length * loc_f_h_advance) * 0.5F);
        break;
    case SGL_ALIGN_RIGHT:
        loc_s_return.f_max_x = par_f_x;
        loc_s_return.f_min_x = loc_s_return.f_max_x - ((SGLfloat) loc_ul_max_length * loc_f_h_advance);
        break;
    default:
        /*
           case SGL_ALIGN_LEFT:
         */
        loc_s_return.f_min_x = par_f_x;
        loc_s_return.f_max_x = loc_s_return.f_min_x + ((SGLfloat) loc_ul_max_length * loc_f_h_advance);
        break;
    }

    loc_s_return.ul_nb_lines = loc_ul_number_of_lines;

    return loc_s_return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetTextMultilineBounds
  DESCRIPTION:
    Function shall return the bounds of a multiline text.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
    par_l_font_id -> font to be taken into account
	par_l_hor_alignment -> horizontal alignment
	par_l_vert_alignment -> vertical alignment
  RETURN:
    None
---------------------------------------------------------------------+*/
sgl_bounding_box sglGetTextMultilineBounds(SGLfloat par_f_x, SGLfloat par_f_y,
                                           const SGLbyte * par_pb_string, SGLlong par_l_string_length,
                                           SGLlong par_l_font_id, SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    sgl_bounding_box loc_s_return;
    SGLbyte loc_b_error_detected = 0U;
    SGLfloat loc_f_x;
    SGLfloat loc_f_y;

    /* Round the position to the closest entire pixel */
    sglConvertPointToPixel(par_f_x, par_f_y, &loc_f_x, &loc_f_y);
    loc_f_x = mth_f_round(loc_f_x);
    loc_f_y = mth_f_round(loc_f_y);
    sglConvertPixelToPoint(loc_f_x, loc_f_y, &par_f_x, &par_f_y);

    loc_s_return.f_min_x = 0.0F;
    loc_s_return.f_max_x = 0.0F;
    loc_s_return.f_min_y = 0.0F;
    loc_s_return.f_max_y = 0.0F;

    /* Check given string is not NULL */
    if (par_pb_string == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check the size of given string is positive */
    if ((par_l_string_length < 0L) || (par_l_font_id < 0L)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
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
        SGLlong loc_l_counter;
        SGLulong loc_ui_mblen;
        const GLubyte *loc_pb_string;

        if (par_l_string_length > (SGLlong) SGLU_MAX_STRING_LENGTH) {
            par_l_string_length = (SGLlong) SGLU_MAX_STRING_LENGTH;
        }
        else {
            /* Nothing to do */
        }

        loc_l_counter = (GLint) par_l_string_length;
        loc_pb_string = par_pb_string;
        par_l_string_length = 0U;

        while (loc_l_counter > 0) {
            if ((*loc_pb_string) != 0U) {
                GLushort loc_us_index = 0U;
                if (*loc_pb_string != SGL_K_CR && *loc_pb_string != SGL_K_LF) {
                    loc_ui_mblen = (GLuint) oglx_get_char_index(&loc_us_index, loc_pb_string);
                    loc_pb_string += loc_ui_mblen;
                    glob_us_string[par_l_string_length] = loc_us_index;
                }
                else {
                    glob_us_string[par_l_string_length] = (GLushort) * loc_pb_string;
                    loc_pb_string++;
                }

                loc_l_counter--;
                par_l_string_length++;
            }
            else {
                loc_l_counter = 0;
            }
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
