/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetRichTextBounds.c
 DESCRIPTION       : sglGetRichTextBounds command shall compute the bounds of a rich text
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
  NAME: oglx_compute_character_bounds
  DESCRIPTION:
    Function shall compute the bounds of a character.
  SCOPE: global
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_ui_string_length -> Global string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_compute_character_bounds(sgl_rich_text_attr * par_rich_text_attr)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;

    const glf_type_character *loc_pr_char;

    par_rich_text_attr->ui_counter--;
    par_rich_text_attr->us_index = (GLushort) (*par_rich_text_attr->pb_string);
    par_rich_text_attr->pb_string++;

    loc_pr_char = GLF_GET_CHAR(par_rich_text_attr->p_current_font, par_rich_text_attr->us_index);

    if (par_rich_text_attr->ui_counter == 0) {
        /* last character */
        par_rich_text_attr->f_width += loc_pr_char->f_width * loc_f_ratio_x;
    }
    else {
        /* other character */
        par_rich_text_attr->f_width += loc_pr_char->f_horizontal_advance * loc_f_ratio_x;
    }

    if (par_rich_text_attr->f_height < loc_pr_char->f_height * loc_f_ratio_y) {
        par_rich_text_attr->f_height = loc_pr_char->f_height * loc_f_ratio_y;
    }
    else {
        /* nothing to do */
    }

    if (par_rich_text_attr->f_max_vert_advance < loc_pr_char->f_vertical_advance * loc_f_ratio_y) {
        par_rich_text_attr->f_max_vert_advance = loc_pr_char->f_vertical_advance * loc_f_ratio_y;
    }
    else {
        /* Nothing to do */
    }

    par_rich_text_attr->us_index = (GLushort) (*par_rich_text_attr->pb_string);

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: glf_get_rich_text_bounds
  DESCRIPTION:
    Function shall compute the rich text bounding box.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pb_string -> string to display
    par_ui_string_length -> length of the string
    par_f_hor_alignment -> horizontal alignment
    par_f_vert_alignment -> vertical alignment
  RETURN:
    sgl_bounding_box -> the computed bounding box
---------------------------------------------------------------------+*/
sgl_bounding_box glf_get_rich_text_bounds(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string,
                                          SGLulong par_ui_string_length, SGLfloat par_f_hor_alignment, SGLfloat par_f_vert_alignment)
{
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_tx;
    SGLfloat loc_f_ty;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_scale_y;
    SGLushort loc_us_repeat_number = 0;
    SGLulong loc_ui_repeat_counter = 0;
    const SGLbyte *loc_pb_repeat_string = SGL_NULL;
    SGLbyte loc_b_first_bounds_value = SGL_TRUE;
    SGLfloat loc_f_min_x = 0.0F;
    SGLfloat loc_f_max_x = 0.0F;
    sgl_bounding_box loc_s_return;
    sgl_rich_text_attr loc_rich_text_attr;
    SGLulong loc_ul_nb_lines = 0UL;
    SGLfloat loc_f_tmp = 0.0F;

    loc_rich_text_attr.f_width = 0.0F;
    loc_rich_text_attr.f_height = 0.0F;
    loc_rich_text_attr.ui_index = 0UL;
    loc_rich_text_attr.l_back_color = -1;
    loc_rich_text_attr.l_fore_color = -1;
    loc_rich_text_attr.b_activated_lines = 0;
    loc_rich_text_attr.ui_last_char = 0U;
    loc_rich_text_attr.b_frame_started = SGL_FALSE;
    loc_rich_text_attr.b_outline_started = SGL_FALSE;
    loc_rich_text_attr.f_max_underline = -100.0F;
    loc_rich_text_attr.f_min_stroke = 100.0F;
    loc_rich_text_attr.f_max_vert_advance = 0.0F;
    loc_rich_text_attr.ui_param_font_id = 0U;
    loc_rich_text_attr.b_reverse_video = SGL_FALSE;
    loc_rich_text_attr.us_line_number = 0;
    loc_rich_text_attr.b_cr_started = SGL_FALSE;
    loc_rich_text_attr.l_rev_color = -1;

    loc_rich_text_attr.ui_current_font_id = glob_pr_sglStatemachine->ui_current_font_id;
    loc_rich_text_attr.p_current_font = glob_pr_sglStatemachine->p_current_font;

    /* Set length */
    loc_rich_text_attr.ui_counter = (GLint) par_ui_string_length;
    loc_rich_text_attr.pb_string = par_pb_string;
    loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);

    /* First cross of the string to prepare the characters decorations and dimensions */
    while (loc_rich_text_attr.ui_counter > 0) {

        switch (loc_rich_text_attr.us_index) {
        case SGL_K_ESC:
        {
            loc_rich_text_attr.ui_counter--;
            loc_rich_text_attr.pb_string++;
            loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);

            switch (loc_rich_text_attr.us_index) {
            case SGL_K_FONT:
            {
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
                oglxSelectFont(loc_rich_text_attr.us_index);
                loc_rich_text_attr.ui_current_font_id = glob_pr_sglStatemachine->ui_current_font_id;
                loc_rich_text_attr.p_current_font = glob_pr_sglStatemachine->p_current_font;
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
            }
                break;
            case SGL_K_FORECOLOR:
            case SGL_K_BACKCOLOR:
            case SGL_K_OUTLINE:
            {
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
            }
                break;
            case SGL_K_REPEAT_BEG:
            {
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
                loc_us_repeat_number = loc_rich_text_attr.us_index;
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_pb_repeat_string = loc_rich_text_attr.pb_string;
                loc_ui_repeat_counter = (SGLulong) (loc_rich_text_attr.ui_counter);
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
            }
                break;
            case SGL_K_REPEAT_END:
            {
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                if (loc_us_repeat_number > 0) {
                    loc_us_repeat_number--;
                    loc_rich_text_attr.ui_counter = (GLint) loc_ui_repeat_counter;
                    loc_rich_text_attr.pb_string = loc_pb_repeat_string;
                }
                else {
                    /* Nothing to do */
                }
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
            }
                break;
            default:
            {
                loc_rich_text_attr.ui_counter--;
                loc_rich_text_attr.pb_string++;
                loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
            }
                break;
            }

        }
            break;
        case SGL_K_CR:
        {
            loc_rich_text_attr.ui_counter--;
            loc_rich_text_attr.pb_string++;
            loc_f_tx = -par_f_hor_alignment * loc_rich_text_attr.f_width;
            if (loc_b_first_bounds_value) {
                loc_f_min_x = loc_f_x + loc_f_tx;
                loc_f_max_x = loc_f_x + loc_f_tx + loc_rich_text_attr.f_width;
                loc_b_first_bounds_value = SGL_FALSE;
            }
            else {
                if (loc_f_min_x > (loc_f_x + loc_f_tx)) {
                    loc_f_min_x = loc_f_x + loc_f_tx;
                }
                else {
                    /* Nothing to do */
                }

                if (loc_f_max_x < (loc_f_x + loc_f_tx + loc_rich_text_attr.f_width)) {
                    loc_f_max_x = loc_f_x + loc_f_tx + loc_rich_text_attr.f_width;
                }
                else {
                    /* Nothing to do */
                }
            }
            loc_rich_text_attr.f_width = 0.0F;
            loc_rich_text_attr.us_line_number++;
            loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
        }
            break;
        case SGL_K_LF:
        {
            loc_rich_text_attr.ui_counter--;
            loc_rich_text_attr.pb_string++;
            loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);
            loc_ul_nb_lines++;
        }
            break;
        default:
        {
            oglx_compute_character_bounds(&loc_rich_text_attr);
        }
            break;
        }
    }

    loc_f_tx = -par_f_hor_alignment * loc_rich_text_attr.f_width;
    if (loc_b_first_bounds_value) {
        loc_f_min_x = loc_f_x + loc_f_tx;
        loc_f_max_x = loc_f_x + loc_f_tx + loc_rich_text_attr.f_width;
    }
    else {
        if (loc_f_min_x > (loc_f_x + loc_f_tx)) {
            loc_f_min_x = loc_f_x + loc_f_tx;
        }
        else {
            /* Nothing to do */
        }

        if (loc_f_max_x < (loc_f_x + loc_f_tx + loc_rich_text_attr.f_width)) {
            loc_f_max_x = loc_f_x + loc_f_tx + loc_rich_text_attr.f_width;
        }
        else {
            /* Nothing to do */
        }
    }

    if (loc_rich_text_attr.us_line_number == 0) {
        loc_f_ty = -(par_f_vert_alignment * loc_rich_text_attr.f_height);
    }
    else {
        loc_f_ty = -(par_f_vert_alignment * loc_rich_text_attr.f_height)
            + (((1.0F - par_f_vert_alignment) * (loc_rich_text_attr.f_height + (loc_rich_text_attr.f_max_vert_advance * loc_f_ratio_y))) *
               (SGLfloat) (loc_rich_text_attr.us_line_number));
    }
    loc_f_y += loc_f_ty;

    loc_s_return.f_min_x = loc_f_min_x;
    loc_f_tmp = ((loc_rich_text_attr.f_height + (loc_rich_text_attr.f_max_vert_advance * loc_f_ratio_y)) * (SGLfloat) (loc_rich_text_attr.us_line_number));
    loc_s_return.f_min_y = loc_f_y - ((loc_rich_text_attr.f_max_vert_advance * loc_f_ratio_y) + loc_f_tmp);
    loc_s_return.f_max_x = loc_f_max_x;
    loc_s_return.f_max_y = loc_f_y + (loc_rich_text_attr.f_height + (2.0F * loc_f_ratio_y));
    loc_s_return.ul_nb_lines = loc_ul_nb_lines + 1UL;

    return loc_s_return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: glfGetRichTextBounds
  DESCRIPTION:
    Function shall compute the string length taking into account of escape sequences
    and then compute the bounding box.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_ui_font_identifier -> font identifier
    par_pb_string -> string to display
    par_ui_string_length -> length of the string
    par_f_hor_alignment -> horizontal alignment
    par_f_vert_alignment -> vertical alignment
  RETURN:
    sgl_bounding_box -> the computed bounding box
---------------------------------------------------------------------+*/
sgl_bounding_box glfGetRichTextBounds(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, const SGLbyte * par_pb_string,
                                      SGLulong par_ui_string_length, SGLfloat par_f_hor_alignment, SGLfloat par_f_vert_alignment)
{
    const SGLbyte *loc_pb_string = par_pb_string;
    SGLulong loc_ui_param_length = par_ui_string_length;
    SGLbyte loc_b_end_of_string = SGL_FALSE;

    par_ui_string_length = 0U;

    while ((!loc_b_end_of_string) && (par_ui_string_length < loc_ui_param_length)) {
        if (loc_pb_string[par_ui_string_length] == 0U) {
            if (par_ui_string_length >= 2U) {
                if ((((loc_pb_string[par_ui_string_length - 1]) == SGL_K_FONT)
                     || ((loc_pb_string[par_ui_string_length - 1]) == SGL_K_FORECOLOR)
                     || ((loc_pb_string[par_ui_string_length - 1]) == SGL_K_BACKCOLOR)
                     || ((loc_pb_string[par_ui_string_length - 1]) == SGL_K_REPEAT_BEG))
                    && ((loc_pb_string[par_ui_string_length - 2]) == SGL_K_ESC)) {
                    par_ui_string_length++;
                }
                else {
                    loc_b_end_of_string = SGL_TRUE;
                }
            }
            else {
                loc_b_end_of_string = SGL_TRUE;
            }
        }
        else {
            par_ui_string_length++;
        }
    }

    /* Reduce the string if it is longer than SGLU_MAX_STRING_LENGTH */
    if (par_ui_string_length > SGLU_MAX_STRING_LENGTH) {
        par_ui_string_length = SGLU_MAX_STRING_LENGTH;
    }
    else {
        /* Nothing to do */
    }

    oglxSelectFont(par_ui_font_identifier);

    return glf_get_rich_text_bounds(par_f_x, par_f_y, par_pb_string, par_ui_string_length, par_f_hor_alignment, par_f_vert_alignment);
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetRichTextBounds
  DESCRIPTION:
    Function shall compute the bounding box of a rich text.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
    par_l_font_id -> initial font index of rich text
    par_l_hor_alignment -> horizontal alignment
    par_l_vert_alignment -> vertical alignment
  RETURN:
    sgl_bounding_box -> The computed bounding box
---------------------------------------------------------------------+*/
sgl_bounding_box sglGetRichTextBounds(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLlong par_l_string_length, SGLlong par_l_font_id,
                                      SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    SGLbyte loc_b_error_detected = 0U;
    sgl_bounding_box loc_s_return;

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

    /* If no error is detected */
    if (!loc_b_error_detected) {
        SGLfloat loc_f_x;
        SGLfloat loc_f_y;

        /* Round the position to the closest entire pixel */
        sglConvertPointToPixel(par_f_x, par_f_y, &loc_f_x, &loc_f_y);
        loc_f_x = mth_f_round(loc_f_x);
        loc_f_y = mth_f_round(loc_f_y);
        sglConvertPixelToPoint(loc_f_x, loc_f_y, &par_f_x, &par_f_y);

        loc_s_return =
            glfGetRichTextBounds(par_f_x, par_f_y, (SGLulong) par_l_font_id, par_pb_string, (SGLulong) par_l_string_length,
                                 ((SGLfloat) par_l_hor_alignment) * 0.5F, ((SGLfloat) par_l_vert_alignment) * 0.5F);
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
