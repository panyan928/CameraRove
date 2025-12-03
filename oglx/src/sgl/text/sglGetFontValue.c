/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetFontValue.c
 DESCRIPTION       : sglGetFontValue command shall return the required font metric.
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
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_get_font_value_detect_error
  DESCRIPTION:
    Function shall detect parameter errors of sglGetFontValue.
  SCOPE: global
  PARAMETERS:
    par_ul_font_id -> to give the font index of the character
    par_b_value_kind -> to specify which metric shall be returned by the function: SGL_CHAR_WIDTH, SGL_CHAR_HEIGHT, SGL_VERTICAL_ADVANCE or SGL_HORIZONTAL_ADVANCE.
  RETURN:
    SGLbool -> SGL_TRUE if an error is detected, SGL_FALSE otherwise.
---------------------------------------------------------------------+*/
SGLbool sgl_get_font_value_detect_error(SGLulong par_ul_font_id, SGLbyte par_b_value_kind)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if (par_ul_font_id >= glob_pr_sglStatemachine->ul_nb_of_fonts) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_b_value_kind != SGL_CHAR_WIDTH) && (par_b_value_kind != SGL_CHAR_HEIGHT)
        && (par_b_value_kind != SGL_VERTICAL_ADVANCE) && (par_b_value_kind != SGL_HORIZONTAL_ADVANCE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetFontValue
  DESCRIPTION:
    Function shall return the required font metric.
  SCOPE: global
  PARAMETERS:
    par_ul_font_id -> to give the font index of the character
    par_ul_char_id -> to give the character index in the font
    par_b_value_kind -> to specify which metric shall be returned by the function: SGL_CHAR_WIDTH, SGL_CHAR_HEIGHT, SGL_VERTICAL_ADVANCE or SGL_HORIZONTAL_ADVANCE.
  RETURN:
  SGLfloat -> The returned metric value.
---------------------------------------------------------------------+*/
SGLfloat sglGetFontValue(SGLulong par_ul_font_id, SGLulong par_ul_char_id, SGLbyte par_b_value_kind)
{
    SGLfloat loc_f_return = 0.0F;
    SGLbool loc_b_error_detected = sgl_get_font_value_detect_error(par_ul_font_id, par_b_value_kind);

    if (!loc_b_error_detected) {
        const glf_type_font *loc_p_input_font = oglx_get_font(par_ul_font_id);
        const glf_type_character *loc_pr_char = GLF_GET_CHAR(loc_p_input_font, par_ul_char_id);

        if (loc_pr_char != SGL_NULL) {
            switch (par_b_value_kind) {
            case SGL_CHAR_WIDTH:
            {
                loc_f_return = loc_pr_char->f_width;
            }
                break;
            case SGL_CHAR_HEIGHT:
            {
                loc_f_return = loc_pr_char->f_height;
            }
                break;
            case SGL_VERTICAL_ADVANCE:
            {
                loc_f_return = loc_pr_char->f_vertical_advance;
            }
                break;
            default:
                /*
                   case SGL_HORIZONTAL_ADVANCE:
                 */
            {
                loc_f_return = loc_pr_char->f_horizontal_advance;
            }
                break;
            }
        }
        else {
            oglxSetError(SGL_ERROR_SGL_GETFONTVALUE, (SGLulong) par_b_value_kind);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_GETFONTVALUE, (SGLulong) par_b_value_kind);
    }
    return loc_f_return;
}

/* End of File ***************************************************************/
