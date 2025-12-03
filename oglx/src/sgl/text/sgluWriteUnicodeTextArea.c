/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteUnicodeTextArea.c
 DESCRIPTION       : sgluWriteUnicodeTextArea command shall draw a text area in unicode format
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
#pragma attol crc_justify_all (Rule M17.4, 2000, "Rule M17.4 is not applied here as we do not know by advance the dimensions of the fonts structure.")
#pragma attol crc_justify_all (Rule M11.5, 2000, "Rule M11.5 is not applied here as the input string is only read, it is not modified.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluWriteUnicodeTextArea
  DESCRIPTION:
    Function shall draw a string.
  SCOPE: global
  PARAMETERS:
    par_f_x_bottom_left -> x-coordinate of the bottom left point
    par_f_y_bottom_left -> y-coordinate of the bottom left point
    par_f_x_top_right -> x-coordinate of the top right point
    par_f_y_top_right -> y-coordinate of the top right point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluWriteUnicodeTextArea(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                              const SGLlong * par_pl_string, SGLlong par_l_string_length)
{
    SGLfloat loc_f_temp;
    SGLulong loc_ul_string_length;
    SGLfloat loc_f_area_width = mth_norm(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_bottom_left);
    SGLfloat loc_f_area_height = mth_norm(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_bottom_left, par_f_y_top_right);
    SGLulong loc_ul_font_id = glob_pr_sglStatemachine->ul_font_id;

    SGLfloat loc_f_prev_v_alignment = glob_pr_sglStatemachine->f_v_alignment;
    SGLbyte loc_b_prev_v_alignment = glob_pr_sglStatemachine->b_v_alignment;
    SGLulong loc_ul_number_of_lines = 0U;
    SGLbyte loc_b_error_detected = 0U;
    SGLulong loc_ul_i = 0UL;
    if ((par_l_string_length >= 0) && (par_pl_string != SGL_NULL)) {
        sgl_line_width *loc_prev_line_width = glob_pr_sglStatemachine->tr_line_width;
        SGLulong loc_prev_line_width_size = glob_pr_sglStatemachine->ul_nb_line_width;
        SGLlong loc_l_line_width_backup = glob_pr_sglStatemachine->l_linewidth_index;
        SGLulong loc_ui_param_length;

        while ((loc_ul_i < (SGLulong) par_l_string_length) && (par_pl_string[loc_ul_i] != 0L) && (!loc_b_error_detected)) {
            if (par_pl_string[loc_ul_i] < 0) {
                loc_b_error_detected = 1U;
            }
            else {
                /* Nothing to do */
            }
            loc_ul_i++;
        }

        if (!loc_b_error_detected) {
            glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
            glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
            glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

            if (par_l_string_length > (SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH) {
                par_l_string_length = (SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH;
            }
            else {
                /* Nothing to do */
            }

            loc_ui_param_length = sglInt32ToUInt32(par_l_string_length);
            par_l_string_length = 0U;

            while (((SGLulong) par_l_string_length < loc_ui_param_length) && (par_pl_string[par_l_string_length] != 0L)) {
                glob_us_string[par_l_string_length] = (SGLushort) par_pl_string[par_l_string_length];
                par_l_string_length++;
            }

            glob_us_string[par_l_string_length] = 0;

            loc_ul_string_length = mth_strlen(glob_us_string, (SGLulong) par_l_string_length);

            if (par_f_x_bottom_left > par_f_x_top_right) {
                loc_f_temp = par_f_x_top_right;
                par_f_x_top_right = par_f_x_bottom_left;
                par_f_x_bottom_left = loc_f_temp;
            }
            else {
                /* Nothing to do */
            }

            if (par_f_y_bottom_left > par_f_y_top_right) {
                loc_f_temp = par_f_y_top_right;
                par_f_y_top_right = par_f_y_bottom_left;
                par_f_y_bottom_left = loc_f_temp;
            }
            else {
                /* Nothing to do */
            }

            loc_ul_number_of_lines = sgl_define_string_lines(glob_us_string, loc_ul_string_length, loc_f_area_width);

            sgl_draw_string_lines(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_top_right, loc_f_area_height,
                                  loc_ul_string_length, loc_ul_font_id, loc_ul_number_of_lines);

            glob_pr_sglStatemachine->f_v_alignment = loc_f_prev_v_alignment;
            glob_pr_sglStatemachine->b_v_alignment = loc_b_prev_v_alignment;

            glob_pr_sglStatemachine->tr_line_width = loc_prev_line_width;
            glob_pr_sglStatemachine->ul_nb_line_width = loc_prev_line_width_size;
            glob_pr_sglStatemachine->l_linewidth_index = loc_l_line_width_backup;
        }
        else {
            oglxSetError(SGL_ERROR_SGLU_WRITEUNICODE_TEXTAREA, 0U);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_WRITEUNICODE_TEXTAREA, 0U);
    }

    return;
}

/* End of File ***************************************************************/
