/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteFloat.c
 DESCRIPTION       : sgluWriteFloat command shall draw a float
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

#define SGL_MAX_DIGITS 20
#define SGL_MAX_INT_FRAC_DIGITS 9

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_add_leading_zeroes
  DESCRIPTION:
    Function shall add leading zeroes to the float.
  SCOPE: global
  PARAMETERS:
    par_b_string -> input float string
    par_l_integer_length -> size of the integer part
    par_l_format_length -> number of digits
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_add_leading_zeroes(SGLbyte * par_b_string, SGLlong * par_l_integer_length, SGLlong par_l_format_length)
{
    SGLlong loc_l_i;
    SGLlong loc_l_diff = (par_l_format_length - (*par_l_integer_length));
    par_b_string[par_l_format_length] = (SGLbyte) 0x0;
    loc_l_i = par_l_format_length - 1;

    while (loc_l_i >= loc_l_diff) {
        par_b_string[loc_l_i] = par_b_string[loc_l_i - loc_l_diff];
        loc_l_i--;
    }

    /* Complete with leading zeros */
    while (loc_l_i >= 0) {
        par_b_string[loc_l_i] = (SGLbyte) 0x30;
        *par_l_integer_length = (*par_l_integer_length) + 1;
        loc_l_i--;
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_truncate_integer_part
  DESCRIPTION:
    Function shall truncate the integer part.
  SCOPE: global
  PARAMETERS:
    par_b_left_part -> the string containing the integer part
    par_l_integer_length -> length of integer part
    par_l_integral_part -> size of integral part
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_truncate_integer_part(SGLbyte * par_b_left_part, SGLlong par_l_integer_length, SGLlong par_l_integral_part)
{
    SGLlong loc_l_diff = par_l_integer_length - par_l_integral_part;
    SGLlong loc_l_i = 0;

    while (loc_l_i < par_l_integral_part) {
        par_b_left_part[loc_l_i] = par_b_left_part[loc_l_i + loc_l_diff];
        loc_l_i++;
    }

    par_b_left_part[par_l_integral_part] = (SGLbyte) 0x0;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_add_sign_character
  DESCRIPTION:
    Function shall add the sign character.
  SCOPE: global
  PARAMETERS:
    par_b_left_part -> the string containing the integer part
    par_l_integer_length -> length of integer part
    par_l_integral_part -> size of integral part
	par_b_sign -> the sign character
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_add_sign_character(SGLbyte * par_b_left_part, SGLlong par_l_integer_length, SGLlong par_l_integral_part, SGLbyte par_b_sign)
{
    SGLlong loc_l_i = par_l_integral_part;
    par_b_left_part[par_l_integer_length + 1] = (SGLbyte) 0x0;

    while (loc_l_i >= 1) {
        par_b_left_part[loc_l_i] = par_b_left_part[loc_l_i - 1];
        loc_l_i--;
    }

    par_b_left_part[loc_l_i] = par_b_sign;

    return;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 20, "Rule M16.7 is not applied here, the function ensures the content of the pointer is not modified.")
#endif
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_change_font_integral_part
  DESCRIPTION:
    Function shall change the font of integral part.
  SCOPE: global
  PARAMETERS:
    par_b_left_part -> the string containing the integer part
    par_l_integer_length -> length of integer part
    par_b_right_part -> the string containing the fractional part
    par_l_fractional_length -> length of fractional part
    par_p_format -> the format of the float
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_change_font_integral_part(SGLbyte * par_b_left_part, SGLlong par_l_integer_length,
                                   SGLbyte * par_b_right_part, SGLlong par_l_fractional_length, sgl_numeric_format * par_p_format)
{
    SGLlong loc_l_i, loc_l_j;

    SGLlong loc_l_shift = -(par_p_format->l_secondFontPos);
    /* Check the change of font has an effect on the drawn value */
    if (loc_l_shift > par_l_integer_length) {
        loc_l_shift = par_l_integer_length;
    }
    else {
        /* Nothing to do */
    }
    /* 1. Shift right characters */
    loc_l_i = par_l_fractional_length - 1;
    while (loc_l_i >= 0) {
        par_b_right_part[loc_l_i + loc_l_shift + 1] = par_b_right_part[loc_l_i];
        loc_l_i--;
    }

    /* 2. Add separator and end right part */
    par_b_right_part[loc_l_shift] = par_p_format->b_separator;
    par_b_right_part[par_l_fractional_length + loc_l_shift + 1] = (SGLbyte) 0x0;

    /* 3. Transfer left characters to the right */
    loc_l_i = par_l_integer_length - 1;
    loc_l_j = loc_l_shift - 1;
    while (loc_l_i >= (par_l_integer_length - loc_l_shift)) {
        par_b_right_part[loc_l_j] = par_b_left_part[loc_l_i];
        loc_l_i--;
        loc_l_j--;
    }

    /* 4. End the left part */
    par_b_left_part[par_l_integer_length - loc_l_shift] = (SGLbyte) 0x0;


    return;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 20, "Rule M16.7 is not applied here, the function ensures the content of the pointer is not modified.")
#endif
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_change_font_fractional_part
  DESCRIPTION:
    Function shall change the font of fractional part.
  SCOPE: global
  PARAMETERS:
    par_b_left_part -> the string containing the integer part
    par_l_integer_length -> length of integer part
    par_b_right_part -> the string containing the fractional part
    par_l_fractional_length -> length of fractional part
    par_p_format -> the format of the float
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_change_font_fractional_part(SGLbyte * par_b_left_part, SGLlong par_l_integer_length, SGLbyte * par_b_right_part, sgl_numeric_format * par_p_format)
{
    SGLlong loc_l_i;

    /* 1. Add separator */
    par_b_left_part[par_l_integer_length] = par_p_format->b_separator;

    /* 2. Transfer right characters to the left */
    loc_l_i = par_l_integer_length + 1;
    while (loc_l_i < (par_l_integer_length + (par_p_format->l_secondFontPos))) {
        par_b_left_part[loc_l_i] = par_b_right_part[loc_l_i - (par_l_integer_length + 1)];
        loc_l_i++;
    }

    /* 3. End left part */
    par_b_left_part[par_l_integer_length + (par_p_format->l_secondFontPos)] = (SGLbyte) 0x0;

    /* 4. Shift left characters */
    loc_l_i = 0;
    while (loc_l_i < par_p_format->l_fractionalPart + (par_p_format->l_secondFontPos - 1)) {
        par_b_right_part[loc_l_i] = par_b_right_part[loc_l_i + (par_p_format->l_secondFontPos - 1)];
        loc_l_i++;
    }

    /* 5. End right part */
    par_b_right_part[par_p_format->l_fractionalPart + (par_p_format->l_secondFontPos - 1)] = (SGLbyte) 0x0;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_separate_1st_2nd_font
  DESCRIPTION:
    Function shall fill left and right part according to given format.
  SCOPE: global
  PARAMETERS:
    par_b_left_part -> the string containing the integer part
    par_l_integer_length -> length of integer part
    par_b_right_part -> the string containing the fractional part 
    par_l_fractional_length -> length of fractional part
    par_p_format -> the format of the float
    par_l_sign -> sign of the float, (-1 if negative, 1 if positive)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_separate_1st_2nd_font(SGLbyte * par_b_left_part, SGLlong par_l_integer_length,
                               SGLbyte * par_b_right_part, SGLlong par_l_fractional_length, sgl_numeric_format * par_p_format, SGLlong par_l_sign)
{
    SGLlong loc_l_i;
    SGLlong loc_l_j;

    SGLbyte loc_b_sign = (SGLbyte) 0x0;
    SGLlong loc_l_integral_part = par_p_format->l_integralPart;


    /* First, integer part shall be formatted */
    if (loc_l_integral_part != par_l_integer_length) {
        if (loc_l_integral_part > par_l_integer_length) {
            if (par_p_format->b_leadingZeros) {
                sgl_add_leading_zeroes(par_b_left_part, &par_l_integer_length, loc_l_integral_part);
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Truncate the integer part */
            sgl_truncate_integer_part(par_b_left_part, par_l_integer_length, loc_l_integral_part);
        }

        /* If there is less than the integer part characters to display, the integer lenght shall be set to the integral part */
        if (loc_l_integral_part < par_l_integer_length) {
            par_l_integer_length = loc_l_integral_part;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    /* Check if negative sign shall be displayed */
    if ((par_l_sign == -1) && ((par_p_format->l_displaySign == (SGLlong) SGL_SIGN_WHEN_NEGATIVE) || (par_p_format->l_displaySign == (SGLlong) SGL_SIGN_ALWAYS))) {
        loc_b_sign = (SGLbyte) 0x2D;
    }
    else {
        /* Nothing to do */
    }

    /* Check if positive sign shall be displayed */
    if ((par_l_sign == 1L) && (par_p_format->l_displaySign == (SGLlong) SGL_SIGN_ALWAYS)) {
        loc_b_sign = (SGLbyte) 0x2B;
    }
    else {
        /* Nothing to do */
    }

    /* Add sign character */
    if (loc_b_sign != ((SGLbyte) 0x0)) {
        sgl_add_sign_character(par_b_left_part, par_l_integer_length, loc_l_integral_part, loc_b_sign);
        par_l_integer_length++;
    }
    else {
        /* Nothing to do */
    }


    /* then, fractional part shall be formatted */
    if (par_p_format->l_fractionalPart != par_l_fractional_length) {
        par_b_right_part[par_p_format->l_fractionalPart] = (SGLbyte) 0x0;
    }
    else {
        /* Nothing to do */
    }

    /* Manage change of font */
    if (par_p_format->l_fractionalPart != 0) {
        /* The change of font is on the integral part */
        if (par_p_format->l_secondFontPos <= 0) {
            sgl_change_font_integral_part(par_b_left_part, par_l_integer_length, par_b_right_part, par_l_fractional_length, par_p_format);
        }
        else {
            sgl_change_font_fractional_part(par_b_left_part, par_l_integer_length, par_b_right_part, par_p_format);
        }
    }
    else {
        /* Special case when there is no fractional part */
        if (par_p_format->l_secondFontPos < 0) {
            SGLlong loc_l_left_end = 0L;
            SGLlong loc_l_right_end = 0L;
            if (par_l_integer_length < -(par_p_format->l_secondFontPos)) {
                loc_l_right_end = par_l_integer_length;
                loc_l_i = par_l_integer_length - 1;
                loc_l_j = loc_l_i;
            }
            else {
                loc_l_right_end = -(par_p_format->l_secondFontPos);
                loc_l_left_end = par_l_integer_length + par_p_format->l_secondFontPos;
                loc_l_i = par_l_integer_length - 1;
                loc_l_j = -(par_p_format->l_secondFontPos) - 1;
            }

            while (loc_l_i >= loc_l_left_end) {
                par_b_right_part[loc_l_j] = par_b_left_part[loc_l_i];
                loc_l_i--;
                loc_l_j--;
            }
            par_b_right_part[loc_l_right_end] = (SGLbyte) 0x0;
            par_b_left_part[loc_l_left_end] = (SGLbyte) 0x0;
        }
        else {
            /* Nothing to do */
        }
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglu_write_float_detected_errors
  DESCRIPTION:
    Function shall check parameters of sgluWriteFloat.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_f_value -> Float to draw
	par_p_format -> format of the float to draw
  RETURN:
    SGL_TRUE if an error is detected, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sglu_write_float_detected_errors(sgl_numeric_format * par_p_format)
{
    SGLbool loc_b_return = SGL_FALSE;

    /* Number of integral digits shall be positive */
    if (par_p_format->l_integralPart < 0L) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Number of fractional digits shall be positive */
    if (par_p_format->l_fractionalPart < 0L) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* The total number of digits plus one separator plus one sign shall not be greater than SGL_MAX_DIGITS */
    if (par_p_format->l_fractionalPart > SGL_MAX_INT_FRAC_DIGITS) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (par_p_format->l_integralPart > SGL_MAX_INT_FRAC_DIGITS) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_p_format->l_secondFontPos < -(par_p_format->l_integralPart))
        || (par_p_format->l_secondFontPos > (par_p_format->l_fractionalPart + 1))) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }


    if ((par_p_format->l_displaySign != (SGLlong) SGL_SIGN_WHEN_NEGATIVE) && (par_p_format->l_displaySign != (SGLlong) SGL_SIGN_ALWAYS)
        && (par_p_format->l_displaySign != (SGLlong) SGL_SIGN_NEVER)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }


    if (loc_b_return) {
        par_p_format->l_integralPart = SGL_MAX_INT_FRAC_DIGITS;
        par_p_format->l_fractionalPart = SGL_MAX_INT_FRAC_DIGITS;
        par_p_format->l_secondFontPos = 0;
        par_p_format->l_displaySign = (SGLlong) SGL_SIGN_ALWAYS;
        par_p_format->b_leadingZeros = SGL_TRUE;
        /* We keep the separator value */
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluWriteFloat
  DESCRIPTION:
    Function shall draw float value as a string.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_f_value -> Float to draw
	par_p_format -> format of the float to draw
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluWriteFloat(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_value, sgl_numeric_format * par_p_format)
{
    SGLlong loc_l_integer_part;
    SGLlong loc_l_fractional_part;
    SGLlong loc_l_leading_zeros;
    SGLfloat loc_f_fractional_part;
    SGLfloat loc_f_fractional_value;
    SGLbyte loc_b_left_part[SGL_MAX_DIGITS];
    SGLbyte loc_b_right_part[SGL_MAX_DIGITS];
    SGLushort loc_us_left_part[SGL_MAX_DIGITS];
    SGLushort loc_us_right_part[SGL_MAX_DIGITS];
    SGLlong loc_l_integer_length;
    SGLlong loc_l_fractional_length;
    SGLulong loc_ul_right_length;
    SGLulong loc_ul_left_length;
    SGLlong loc_l_sign;
    GLfloat loc_f_width1;
    GLfloat loc_f_width2;
    GLfloat loc_f_height1;
    GLfloat loc_f_x;
    GLfloat loc_f_y;
    SGLulong loc_ul_i;


    /* Store first font text attributes */
    SGLlong loc_l_prev_color = glob_pr_sglStatemachine->l_color_index;
    SGLulong loc_ul_prev_font_id = glob_pr_sglStatemachine->ul_font_id;
    SGLlong loc_l_prev_second_font_id = glob_pr_sglStatemachine->l_second_font_id;
    SGLlong loc_l_prev_linewidth = glob_pr_sglStatemachine->l_current_font_linewidth_index;

    /* Store previous alignment */
    SGLfloat loc_f_prev_h_alignment = glob_pr_sglStatemachine->f_h_alignment;
    SGLfloat loc_f_prev_v_alignment = glob_pr_sglStatemachine->f_v_alignment;

    SGLbool loc_b_error_detected = sglu_write_float_detected_errors(par_p_format);

    SGLlong loc_l_line_width_backup = glob_pr_sglStatemachine->l_linewidth_index;
    sgl_line_width *loc_prev_line_width = glob_pr_sglStatemachine->tr_line_width;
    SGLulong loc_prev_line_width_size = glob_pr_sglStatemachine->ul_nb_line_width;

    /* Change the coordinate of the first line to match exactly one pixel */
    oglxGetEntirePixelCoordinate(par_f_x, par_f_y, &par_f_x, &par_f_y);

    glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
    glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
    glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

    if (loc_b_error_detected) {
        oglxSetError(SGL_ERROR_SGLU_WRITEFLOAT, 0U);
    }
    else {
        /* Nothing to do */
    }

    if ((par_f_value > SGL_MAX_INT32_AS_FLOAT) || (par_f_value < -SGL_MAX_INT32_AS_FLOAT)) {
        glob_pr_sglStatemachine->ul_font_id = INVALID_FONT_ID;
        glob_pr_sglStatemachine->l_second_font_id = (SGLlong) INVALID_FONT_ID;
        par_f_value = 0.0F;
    }
    else {
        /* Nothing to do */
    }

    /* Round the value to the closest displayed float, avoid truncation */
    if (par_f_value < 0.0F) {
        loc_l_sign = -1;
        par_f_value = par_f_value - SGLfloat_div(0.5F, (SGLfloat) (mth_exponent(10L, par_p_format->l_fractionalPart)));
    }
    else {
        loc_l_sign = 1;
        par_f_value = par_f_value + SGLfloat_div(0.5F, (SGLfloat) (mth_exponent(10L, par_p_format->l_fractionalPart)));
    }

    /* Compute integral and fractional parts as integers */
    loc_l_integer_part = (SGLlong) par_f_value;

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 10, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    /* Compute the number of leading zeros of fractional part */
    loc_f_fractional_part = mth_f_abs(par_f_value) - mth_f_abs((SGLfloat) loc_l_integer_part);
    loc_f_fractional_value = loc_f_fractional_part * ((SGLfloat) mth_exponent(10L, par_p_format->l_fractionalPart));

    loc_l_fractional_part = (SGLlong) loc_f_fractional_value;

    loc_l_leading_zeros = 0;

    loc_f_fractional_part = loc_f_fractional_part * 10.0F;
    while ((loc_f_fractional_part < 1.0F) && (loc_l_leading_zeros < par_p_format->l_fractionalPart)) {
        loc_f_fractional_part = loc_f_fractional_part * 10.0F;
        loc_l_leading_zeros++;
    }

    /* Convert integral and fractional parts to strings */
    mth_integer_to_string(loc_l_integer_part, loc_b_left_part, &loc_l_integer_length);
    mth_integer_to_string(loc_l_fractional_part, loc_b_right_part, &loc_l_fractional_length);

    /* If fractional part starts with zeroes, it shall be added */
    if (loc_l_leading_zeros > 0) {
        sgl_add_leading_zeroes(loc_b_right_part, &loc_l_fractional_length, loc_l_fractional_length + loc_l_leading_zeros);
    }
    else {
        /* Nothing to do */
    }

    /* Separate First font from second font in left and right parts */
    sgl_separate_1st_2nd_font(loc_b_left_part, loc_l_integer_length, loc_b_right_part, loc_l_fractional_length, par_p_format, loc_l_sign);

    for (loc_ul_i = 0; loc_ul_i < SGL_MAX_DIGITS; loc_ul_i++) {
        loc_us_right_part[loc_ul_i] = (SGLushort) (loc_b_right_part[loc_ul_i]);
        loc_us_left_part[loc_ul_i] = (SGLushort) loc_b_left_part[loc_ul_i];
    }

    loc_ul_right_length = mth_strlen(loc_us_right_part, SGL_MAX_DIGITS);
    loc_ul_left_length = mth_strlen(loc_us_left_part, SGL_MAX_DIGITS);

    if (loc_ul_left_length == 0UL) {
        /* Apply second font text attributes */
        oglxUpdateColor(glob_pr_sglStatemachine->l_second_font_color_id);
        sglIndexFonti(glob_pr_sglStatemachine->l_second_font_id);
        sglIndexFontLineWidthi(glob_pr_sglStatemachine->l_second_font_line_width);
        ogl_sgluWriteText(par_f_x, par_f_y, loc_us_right_part, (SGLlong) loc_ul_right_length);
    }
    else {
        if (loc_ul_right_length == 0UL) {
            ogl_sgluWriteText(par_f_x, par_f_y, loc_us_left_part, (SGLlong) loc_ul_left_length);
        }
        else {
            loc_f_width1 = str_get_string_width(loc_us_left_part, loc_ul_left_length, loc_ul_prev_font_id, SGL_FALSE);
            loc_f_width2 = str_get_string_width(loc_us_right_part, loc_ul_right_length, (SGLulong) glob_pr_sglStatemachine->l_second_font_id, SGL_TRUE);
            loc_f_height1 = str_get_string_height(loc_us_left_part, loc_ul_left_length, loc_ul_prev_font_id);

            /* take into account of horizontal alignment */
            switch (glob_pr_sglStatemachine->b_h_alignment) {
            case SGL_ALIGN_CENTER:
                loc_f_x = par_f_x - ((loc_f_width1 + loc_f_width2) * 0.5F);

                glob_pr_sglStatemachine->f_h_alignment = 0.0F;

                /* Draw the left part */
                ogl_sgluWriteText(loc_f_x, par_f_y, loc_us_left_part, (SGLlong) loc_ul_left_length);
                break;
            case SGL_ALIGN_RIGHT:
                loc_f_x = par_f_x - (loc_f_width1 + loc_f_width2);

                glob_pr_sglStatemachine->f_h_alignment = 0.0F;

                /* Draw the left part */
                ogl_sgluWriteText(loc_f_x, par_f_y, loc_us_left_part, (SGLlong) loc_ul_left_length);
                break;
            case SGL_ALIGN_LEFT:
            default:
                loc_f_x = par_f_x;
                /* Draw the left part */
                ogl_sgluWriteText(loc_f_x, par_f_y, loc_us_left_part, (SGLlong) loc_ul_left_length);
                break;
            }

            /* Apply second font text attributes */
            sglIndexColori(glob_pr_sglStatemachine->l_second_font_color_id);
            sglIndexFonti(glob_pr_sglStatemachine->l_second_font_id);
            sglIndexFontLineWidthi(glob_pr_sglStatemachine->l_second_font_line_width);

            /* take into account of vertical alignment */
            switch (glob_pr_sglStatemachine->b_v_alignment) {
            case SGL_ALIGN_MIDDLE:
                loc_f_y = par_f_y - (loc_f_height1 * 0.5F);

                glob_pr_sglStatemachine->f_v_alignment = 0.0F;
                glob_pr_sglStatemachine->b_v_alignment = SGL_ALIGN_BOTTOM;
                break;
            case SGL_ALIGN_TOP:
                loc_f_y = par_f_y - loc_f_height1;

                glob_pr_sglStatemachine->f_v_alignment = 0.0F;
                glob_pr_sglStatemachine->b_v_alignment = SGL_ALIGN_BOTTOM;
                break;
            case SGL_ALIGN_BOTTOM:
            default:
                loc_f_y = par_f_y;
                break;
            }
            /* Draw the right part */
            ogl_sgluWriteText(loc_f_x + loc_f_width1, loc_f_y, loc_us_right_part, (SGLlong) loc_ul_right_length);
        }

    }

    /* Restore first font text attributes */
    sglIndexColori(loc_l_prev_color);
    sglIndexFonti((SGLlong) loc_ul_prev_font_id);
    sglIndexFontLineWidthi(loc_l_prev_linewidth);
    glob_pr_sglStatemachine->l_second_font_id = loc_l_prev_second_font_id;

    /* Restore previous alignment */
    glob_pr_sglStatemachine->f_h_alignment = loc_f_prev_h_alignment;
    glob_pr_sglStatemachine->f_v_alignment = loc_f_prev_v_alignment;

    glob_pr_sglStatemachine->tr_line_width = loc_prev_line_width;
    glob_pr_sglStatemachine->ul_nb_line_width = loc_prev_line_width_size;
    glob_pr_sglStatemachine->l_linewidth_index = loc_l_line_width_backup;

    return;
}

/* End of File ***************************************************************/
