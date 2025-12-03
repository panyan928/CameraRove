/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : str_functions.c
 DESCRIPTION       : This file provides utilitary functions for strings
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Protected interfaces +*/
#include "sgl.h"
#include "sgl_private.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M17.4, 2000, "Rule M17.4 is not applied here as we do not know by advance the dimensions of the fonts structure.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_word_separator
  DESCRIPTION:
    Function shall check if given character is a word separator 
  SCOPE: global
  PARAMETERS:
    par_b_char -> the character to check
  RETURN:
    SGLbyte -> SGL_TRUE if given character is a separator, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbyte sgl_is_word_separator(SGLushort par_us_char)
{
    SGLulong loc_ul_i = 0;
    SGLbyte loc_b_return = SGL_FALSE;

    while ((loc_ul_i < glob_pr_sglStatemachine->ul_nb_word_separators) && (!loc_b_return)) {
        if ((SGLushort) (glob_pr_sglStatemachine->tb_word_separators[loc_ul_i]) == par_us_char) {
            loc_b_return = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
        loc_ul_i++;
    }

    return loc_b_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_line_separator
  DESCRIPTION:
    Function shall check if given character is a line separator 
  SCOPE: global
  PARAMETERS:
    par_b_char -> the character to check
  RETURN:
    SGLbyte -> SGL_TRUE if given character is a separator, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbyte sgl_is_line_separator(SGLushort par_us_char)
{
    SGLulong loc_ul_i = 0;
    SGLbyte loc_b_return = SGL_FALSE;

    while ((loc_ul_i < glob_pr_sglStatemachine->ul_nb_line_separators) && (!loc_b_return)) {
        if ((SGLushort) (glob_pr_sglStatemachine->tb_line_separators[loc_ul_i]) == par_us_char) {
            loc_b_return = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
        loc_ul_i++;
    }

    return loc_b_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: str_get_string_width
  DESCRIPTION:
    Function shall compute the width of a line.
  SCOPE: global
  PARAMETERS:
    par_pus_string -> the input string
    par_ui_string_length -> string length
    par_ui_font_id -> font index
	par_b_count_correction -> correction taken into account or not
  RETURN:
    SGLfloat -> the computed width
---------------------------------------------------------------------+*/
SGLfloat str_get_string_width(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id, SGLbool par_b_count_correction)
{
    SGLulong loc_l_counter;
    SGLushort *loc_pus_string;
    const glf_type_font *loc_p_font = oglx_get_font(par_ui_font_id);

    /* fonts are scaleable */
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_width = 0.0F;
    SGLfloat loc_f_correction = 0.0F;
    GLushort loc_us_index = 0U;

    loc_l_counter = 0;
    loc_pus_string = par_pus_string;

    /* For each character */
    while (loc_l_counter < par_ui_string_length) {
        const glf_type_character *loc_pr_char;
        /* Add a character to the box */
        loc_us_index = (GLushort) (loc_pus_string[loc_l_counter]);

        loc_pr_char = GLF_GET_CHAR(loc_p_font, loc_us_index);

        if (loc_pr_char != SGL_NULL) {
            loc_f_width += (loc_pr_char->f_horizontal_advance * loc_f_ratio_x);
            loc_f_correction = (loc_pr_char->f_horizontal_advance * loc_f_ratio_x) - (loc_pr_char->f_width * loc_f_ratio_x);
        }
        else {
            /* Nothing to do */
        }
        loc_l_counter++;
    }

    if (par_b_count_correction) {
        loc_f_width -= loc_f_correction;
    }
    else {
        /* Nothing to do */
    }

    return loc_f_width;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: str_get_string_height
  DESCRIPTION:
    Function shall compute the height of a line.
  SCOPE: global
  PARAMETERS:
    par_pus_string -> the input string
    par_ui_string_length -> string length
    par_ui_font_id -> font index
  RETURN:
    SGLfloat -> the computed height
---------------------------------------------------------------------+*/
SGLfloat str_get_string_height(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id)
{
    SGLulong loc_l_counter;
    SGLushort *loc_pus_string;
    const glf_type_font *loc_p_font = oglx_get_font(par_ui_font_id);
    const glf_type_character *loc_pr_char;

    /* fonts are scaleable */
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLfloat loc_f_height = 0.0F;
    GLushort loc_us_index = 0U;

    loc_l_counter = 0U;
    loc_pus_string = par_pus_string;

    /* For each character */
    while (loc_l_counter < par_ui_string_length) {
        /* Add a character to the box */
        loc_us_index = (GLushort) (loc_pus_string[loc_l_counter]);

        loc_pr_char = GLF_GET_CHAR(loc_p_font, loc_us_index);

        if (loc_pr_char != SGL_NULL) {
            if ((loc_pr_char->f_height * loc_f_ratio_y) > loc_f_height) {
                loc_f_height = loc_pr_char->f_height * loc_f_ratio_y;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }

        loc_l_counter++;
    }

    return loc_f_height;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: str_get_string_advance
  DESCRIPTION:
    Function shall compute the vertical or horizontal advance of a line.
  SCOPE: global
  PARAMETERS:
    par_pus_string -> string to be drawn
    par_ui_string_length -> string length
    par_ui_font_id -> font index
    par_b_vertical -> Boolean to select advance value to return, return horizontal advance for SGL_FASLE, vertical advance otherwise
  RETURN:
    SGLfloat -> the computed advance
---------------------------------------------------------------------+*/
SGLfloat str_get_string_advance(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id, SGLbyte par_b_vertical)
{
    SGLulong loc_l_counter;
    SGLushort *loc_pus_string;
    const glf_type_font *loc_p_font = oglx_get_font(par_ui_font_id);
    const glf_type_character *loc_pr_char;

    /* fonts are scaleable */
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_advance = 0.0F;
    GLushort loc_us_index = 0U;

    loc_l_counter = 0U;
    loc_pus_string = par_pus_string;

    /* For each character */
    while (loc_l_counter < par_ui_string_length) {
        /* Add a character to the box */
        loc_us_index = (GLushort) (loc_pus_string[loc_l_counter]);

        loc_pr_char = GLF_GET_CHAR(loc_p_font, loc_us_index);

        if (loc_pr_char != SGL_NULL) {
            if (par_b_vertical) {
                if ((loc_pr_char->f_vertical_advance * loc_f_ratio_y) > loc_f_advance) {
                    loc_f_advance = loc_pr_char->f_vertical_advance * loc_f_ratio_y;
                }
                else {
                    /* Nothing to do */
                }
            }
            else {
                if ((loc_pr_char->f_horizontal_advance * loc_f_ratio_x) > loc_f_advance) {
                    loc_f_advance = loc_pr_char->f_horizontal_advance * loc_f_ratio_x;
                }
                else {
                    /* Nothing to do */
                }
            }
        }
        else {
            /* Nothing to do */
        }

        loc_l_counter++;
    }

    return loc_f_advance;
}

/* End of File ***************************************************************/
