/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteUnicodeText.c
 DESCRIPTION       : sgluWriteUnicodeText command shall draw a string
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
  NAME: sgluWriteUnicodeText
  DESCRIPTION:
    Function shall draw a string.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pl_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluWriteUnicodeText(SGLfloat par_f_x, SGLfloat par_f_y, const SGLlong * par_pl_string, SGLlong par_l_string_length)
{
    SGLbyte loc_b_error_detected = 0U;
    SGLulong loc_ul_i = 0UL;

    /* Check given string is not NULL */
    if ((par_pl_string == SGL_NULL) || (par_l_string_length < 0L)) {
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

    /* If no error is detected */
    if (!loc_b_error_detected) {
        sgl_line_width *loc_prev_line_width = glob_pr_sglStatemachine->tr_line_width;
        SGLulong loc_prev_line_width_size = glob_pr_sglStatemachine->ul_nb_line_width;
        SGLlong loc_l_line_width_backup = glob_pr_sglStatemachine->l_linewidth_index;
        SGLbool loc_b_multiline = SGL_FALSE;
        SGLulong loc_ui_param_length = sglInt32ToUInt32(par_l_string_length);

        glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
        glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

        /* Check the length is not too big */
        if (par_l_string_length > (SGLlong) SGLU_MAX_STRING_LENGTH) {
            loc_ui_param_length = (SGLlong) SGLU_MAX_STRING_LENGTH;
            oglxSetError(SGL_ERROR_SGLU_WRITEUNICODETEXT, (SGLulong) par_l_string_length);
        }
        else {
            /* Nothing to do */
        }

        par_l_string_length = 0U;

        while (((SGLulong) par_l_string_length < loc_ui_param_length) && (par_pl_string[par_l_string_length] != 0L)) {
            glob_us_string[par_l_string_length] = (SGLushort) par_pl_string[par_l_string_length];
            if ((glob_us_string[par_l_string_length] == (SGLushort) SGL_K_CR) || (glob_us_string[par_l_string_length] == (SGLushort) SGL_K_LF)) {
                loc_b_multiline = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }

            par_l_string_length++;
        }
        glob_us_string[par_l_string_length] = 0;

        if (glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX) {
            if (loc_b_multiline) {
                ogl_sgluWriteTextMultiline(par_f_x, par_f_y, glob_us_string, (SGLulong) par_l_string_length);
            }
            else {
                ogl_sgluWriteText(par_f_x, par_f_y, glob_us_string, (SGLulong) par_l_string_length);
            }
        }
        else {
            /* Nothing to do */
        }

        glob_pr_sglStatemachine->tr_line_width = loc_prev_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = loc_prev_line_width_size;
        glob_pr_sglStatemachine->l_linewidth_index = loc_l_line_width_backup;
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_WRITEUNICODETEXT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
