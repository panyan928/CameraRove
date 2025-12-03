/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglx_fonts_encoding.c
 DESCRIPTION       : Set of functions used to draw bitmap and stroke fonts
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
#include "glf.h"
#include "mth.h"

#define HALOING_WIDTH 1.0F

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M17.4, 2000, "Rule M17.4 is not applied here as we do not know by advance the dimensions of the fonts structure.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


#define UTF_8_1_CHAR		0x7F
#define UTF_8_2_CHAR		0xC0
#define UTF_8_3_CHAR		0xE0
#define UTF_8_4_CHAR		0xF0
#define UTF_8_PREFIX		0x80
#define GBK_CHAR		0x80

extern glf_type_font *getDefaultFont(void);

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_font
  DESCRIPTION:
    Function shall return the font associated to index, or the default font.
  SCOPE: global
  PARAMETERS:
    par_ul_index -> index of font
  RETURN:
    const glf_type_font * -> Font
---------------------------------------------------------------------+*/
const glf_type_font *oglx_get_font(SGLulong par_ul_index)
{
    const glf_type_font *result = SGL_NULL;
    if (par_ul_index < glob_pr_sglStatemachine->ul_nb_of_fonts) {
        result = glob_pr_sglStatemachine->pr_font_table + par_ul_index;
    }
    else {
        result = getDefaultFont();
    }
    return result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_char_index
  DESCRIPTION:
    Function shall convert unicode character to character index.
  SCOPE: global
  PARAMETERS:
    par_pus_widechar -> Output index of unicode character at the begining of the string
    par_pub_string -> Current string
  RETURN:
    SGLshort -> Offset to next character
---------------------------------------------------------------------+*/
SGLushort oglx_get_char_index(GLushort * par_pus_widechar, const GLubyte * par_pub_string)
{
    GLushort loc_widechar = 0U;
    SGLushort i = 0U;
    GLubyte loc_char = par_pub_string[0];
    SGLushort loc_us_return = 0;
    GLushort loc_us_value;

    switch (glob_pr_sglStatemachine->b_charset_encoding) {
    case SGL_FORMAT_UTF8:
    {
        /* Check the UTF-8 character size */
        if ((loc_char & UTF_8_4_CHAR) == UTF_8_4_CHAR) {
            /* Characters coded on 4 bytes cannot be managed by OGLX as font table manages SGLushort indexes (0 to 65535) */
            /* Default character is drawn */
            loc_us_return = 4;
            if ((par_pus_widechar) != SGL_NULL) {
                *par_pus_widechar = 0;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            if ((loc_char & UTF_8_3_CHAR) == UTF_8_3_CHAR) {
                /* The character is on 3 bytes, the index shall be built from the 3 bytes */
                loc_us_value = (GLushort) (((GLushort) (loc_char & ~UTF_8_3_CHAR)) << 12);
                loc_char = par_pub_string[1];
                loc_us_value = (GLushort) (loc_us_value | (((GLushort) (loc_char & ~UTF_8_PREFIX)) << 6));
                loc_char = par_pub_string[2];
                loc_us_value = (GLushort) (loc_us_value | ((GLushort) (loc_char & ~UTF_8_PREFIX)));
                loc_us_return = 3;
                if ((par_pus_widechar) != SGL_NULL) {
                    *par_pus_widechar = loc_us_value;
                }
                else {
                    /* Nothing to do */
                }
            }
            else {
                if ((loc_char & UTF_8_2_CHAR) == UTF_8_2_CHAR) {
                    /* The character is on 2 bytes, the index shall be built from the 2 bytes */
                    loc_us_value = (GLushort) (((GLushort) (loc_char & ~UTF_8_2_CHAR)) << 6);
                    loc_char = par_pub_string[1];
                    loc_us_value = (GLushort) (loc_us_value | ((GLushort) (loc_char & ~UTF_8_PREFIX)));
                    loc_us_return = 2;

                    if ((par_pus_widechar) != SGL_NULL) {
                        *par_pus_widechar = loc_us_value;
                    }
                    else {
                        /* Nothing to do */
                    }
                }
                else {
                    /* The character is on 1 byte, it can be kept as this */
                    if ((par_pus_widechar) != SGL_NULL) {
                        *par_pus_widechar = loc_char;
                    }
                    else {
                        /* Noting to do */
                    }
                    loc_us_return = 1;
                }
            }
        }
    }
        break;
    case SGL_FORMAT_GBK:
    {
        /* Get par_string (GBK) */
        if ((loc_char & GBK_CHAR) == GBK_CHAR) {
            loc_widechar = (GLushort) (par_pub_string[0] * 256U + par_pub_string[1]);
            if ((par_pus_widechar) != SGL_NULL) {
                *par_pus_widechar = loc_widechar;
            }
            else {
                /* Nothing to do */
            }
            loc_us_return = 2;
        }
        else {
            loc_widechar = loc_char;
            if ((par_pus_widechar) != SGL_NULL) {
                *par_pus_widechar = loc_widechar;
            }
            else {
                /* Nothing to do */
            }
            loc_us_return = 1;
        }
    }
        break;
    case SGL_FORMAT_ASCII:
    default:
    {
        /* C99 ASCII with \uXXXX escape sequences, denoting Unicode characters */

        /* First, read \u */
        if ((loc_char != (GLubyte) '\\') || (par_pub_string[1] != (GLubyte) 'u')) {
            if ((par_pus_widechar) != SGL_NULL) {
                *par_pus_widechar = loc_char;
            }
            else {
                /* Nothing to do */
            }
            loc_us_return = 1;
        }
        else {
            i = 2;

            /* Read the unicode value */
            while ((i < 6U) && (loc_us_return == 0U)) {
                loc_char = par_pub_string[i];
                if ((loc_char >= (GLubyte) '0') && (loc_char <= (GLubyte) '9')) {
                    loc_char -= (GLubyte) '0';
                }
                else {
                    if ((loc_char >= (GLubyte) 'A') && (loc_char <= (GLubyte) 'F')) {
                        loc_char -= ((GLubyte) 'A' - 10U);
                    }
                    else {
                        if ((loc_char >= (GLubyte) 'a') && (loc_char <= (GLubyte) 'f')) {
                            loc_char -= ((GLubyte) 'a' - 10U);
                        }
                        else {
                            if (par_pus_widechar != SGL_NULL) {
                                *par_pus_widechar = (GLubyte) '\\';
                            }
                            else {
                                /* Nothing to do */
                            }
                            loc_us_return = 1;
                        }
                    }
                }

                if (loc_us_return == 0) {
                    /* Convert unicode value to character index */
                    loc_widechar |= (((GLushort) loc_char) << (4U * (5U - i)));
                }
                else {
                    /* Nothing to do */
                }

                i++;
            }

            if (loc_us_return == 0) {
                if (par_pus_widechar != SGL_NULL) {
                    /* Write the character index to the output index */
                    *par_pus_widechar = loc_widechar;
                }
                else {
                    /* Nothing to do */
                }
                /* Next character is 6 characters farer */
                loc_us_return = 6;
            }
            else {
                /* Nothing to do */
            }
        }
    }
        break;
    }
    return loc_us_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_fonts_draw_stroke_character
  DESCRIPTION:
    Function shall draw given stroke character.
  SCOPE: global
  PARAMETERS:
    par_pr_font -> The font to draw
    par_pr_char_stroke -> The character to draw
    par_f_x -> X origin of the character
    par_f_y -> Y origin of the character
    par_f_ratio_x -> X ratio
    par_f_ratio_y -> Y ratio
    par_ui_current_font_linewidth_index -> Line width to apply
    par_ui_current_font_color_index -> Index of color to apply
    par_b_haloing -> Haloing or not
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_fonts_draw_stroke_character(const glf_type_font * par_pr_font, const glf_type_character_stroke * par_pr_char_stroke,
                                      SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y,
                                      SGLulong par_ui_current_font_linewidth_index, SGLulong par_ui_current_font_color_index, SGLbool par_b_haloing)
{
    /* Get character description */
    SGLbyte *loc_char_description;
    SGLulong loc_ui_nb_line_count = 0U;
    SGLulong loc_ui_nb_line;
    SGLfloat loc_f_ratio_x = par_f_ratio_x;
    SGLfloat loc_f_ratio_y = par_f_ratio_y;

    loc_char_description = GLF_GET_STROKE_DATA(par_pr_font) + par_pr_char_stroke->ui_offset_to_description;


    sglIndexLineWidthi((SGLlong) par_ui_current_font_linewidth_index);

    if (par_b_haloing == 0U) {
        oglxUpdateColor((SGLlong) par_ui_current_font_color_index);
    }
    else {
        glob_pr_sglStatemachine->b_haloing_state = SGL_HALOED_PART_ONLY;
    }

    /* Read number of lines */
    loc_ui_nb_line = *(SGLulong *) loc_char_description;
    loc_char_description += sizeof(SGLulong);

    /* Draw each line */
    while (loc_ui_nb_line_count < loc_ui_nb_line) {
        SGLfloat loc_f_point_x = 0.0F;
        SGLfloat loc_f_point_y = 0.0F;
        SGLfloat loc_f_y_minus_1 = 0.0F;
        SGLfloat loc_f_y_plus_1 = 0.0F;
        SGLfloat loc_f_y_adjustment = 0.6F * loc_f_ratio_y;
        SGLushort loc_us_nb_vertex_count = 0U;
        SGLushort loc_us_line_type;
        SGLushort loc_us_nb_vertex;

        /* Read line type */
        loc_us_line_type = *(SGLushort *) loc_char_description;
        loc_char_description += sizeof(SGLushort);

        /* Read number of vertex */
        loc_us_nb_vertex = *(SGLushort *) loc_char_description;
        loc_char_description += sizeof(SGLushort);

        /* Set current line type */
        sglBegin((SGLbyte) loc_us_line_type);


        /* Process each vertex */
        while (loc_us_nb_vertex_count < loc_us_nb_vertex) {
            SGLfloat *loc_pr_point = (SGLfloat *) loc_char_description;
            SGLfloat loc_f_diff = 0.0F;
            loc_f_point_x = loc_pr_point[0] * loc_f_ratio_x;
            loc_f_point_y = loc_pr_point[1] * loc_f_ratio_y;

            /* In case of a line loop, Vertex has not to be adjusted */
            if (loc_us_line_type == GLF_LINE_LOOP) {
                sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
            }
            else {
                /* If the vertex is not an extremity of the line */
                if ((loc_us_nb_vertex_count > 0U) && (loc_us_nb_vertex_count < (loc_us_nb_vertex - 1U))) {
                    SGLfloat *loc_pr_point_plus_1 = loc_pr_point + 2U;
                    loc_f_y_plus_1 = loc_pr_point_plus_1[1] * loc_f_ratio_y;

                    /* If the two consecutive lines change of direction */
                    if ((((loc_f_point_y - loc_f_y_minus_1) * (loc_f_y_plus_1 - loc_f_point_y)) < 0.0F)) {
                        loc_f_diff = loc_f_point_y - loc_f_y_minus_1;
                        /* If current vertex is higher than previous one */
                        if (loc_f_diff > 0.0F) {
                            /* Add a half pixel in Y */
                            sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_y_adjustment + loc_f_point_y);
                        }
                        else {
                            /* Else reduce by a half pixel in Y */
                            sglVertex2f(par_f_x + loc_f_point_x, (par_f_y - loc_f_y_adjustment) + loc_f_point_y);
                        }
                    }
                    else {
                        /* Do not adjust vertex */
                        sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
                    }
                }
                else {
                    /* If current vertex is the first of a line */
                    if (loc_us_nb_vertex_count == 0U) {
                        SGLbool loc_b_compare_y_plus_1;
                        SGLfloat *loc_pr_point_plus_1 = loc_pr_point + 2U;
                        loc_f_y_plus_1 = loc_pr_point_plus_1[1] * loc_f_ratio_y;

                        loc_b_compare_y_plus_1 = sgluFloatIsEqual(loc_f_point_y, loc_f_y_plus_1);
                        /* If current line is horizontal */
                        if (loc_b_compare_y_plus_1) {
                            /* Do not adjust vertex */
                            sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
                        }
                        else {
                            loc_f_diff = loc_f_y_plus_1 - loc_f_point_y;
                            /* Else if current line is a raising one */
                            if (loc_f_diff > 0.0F) {
                                /* Reduce Vertex by a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, (par_f_y - loc_f_y_adjustment) + loc_f_point_y);
                            }
                            else {
                                /* Else add a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_y_adjustment + loc_f_point_y);
                            }
                        }
                    }
                    else {
                        /* Else current vertex is the last one */
                        SGLbool loc_b_compare_y_minus_1 = sgluFloatIsEqual(loc_f_point_y, loc_f_y_minus_1);
                        /* If current line is horizontal */
                        if (loc_b_compare_y_minus_1) {
                            /* Do not adjust vertex */
                            sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
                        }
                        else {
                            loc_f_diff = loc_f_point_y - loc_f_y_minus_1;
                            /* Else if current line is a lowering one */
                            if (loc_f_diff > 0.0F) {
                                /* Add a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_y_adjustment + loc_f_point_y);
                            }
                            else {
                                /* Else reduce Vertex by a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, (par_f_y - loc_f_y_adjustment) + loc_f_point_y);
                            }
                        }
                    }
                }
            }
            loc_char_description += 2U * sizeof(GLfloat);
            loc_f_y_minus_1 = loc_f_point_y;
            loc_us_nb_vertex_count++;
        }
        sglEnd();

        loc_ui_nb_line_count++;
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_fonts_draw_bitmap_character
  DESCRIPTION:
    Function shall draw a stroke character found according to given index and table and return the associated horizontal advance.
  SCOPE: global
  PARAMETERS:
    par_pr_font -> The font to draw
    par_pr_char -> The character to draw
    par_f_x -> X origin of the character
    par_f_y -> Y origin of the character
    par_ui_font_linewidth -> Line width to apply
    par_f_ratio_x -> X ratio
    par_f_ratio_y -> Y ratio
    par_b_haloing -> haloing enabled or not
  RETURN:
    none
---------------------------------------------------------------------+*/
void oglx_fonts_draw_bitmap_character(const glf_type_font * par_pr_font, const glf_type_character_bitmap * par_pr_char,
                                      SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_linewidth,
                                      SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y, SGLbool par_b_haloing)
{
    sgl_vector2d loc_t_vertex_array[4];

    const glf_type_u_v_couple *loc_pr_char_mapping;

    /* Compute rectangle coordinates */
    SGLfloat loc_f_x1;
    SGLfloat loc_f_y1;
    SGLfloat loc_f_x2;
    SGLfloat loc_f_y2;

    sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;

    SGLbool b_halo_width_available;

    if (par_b_haloing) {
        b_halo_width_available = (SGLbool) (GLF_GET_DELTA_HALO_WIDTH(par_pr_font, (par_ui_font_linewidth - 2)) != 0);
    }
    else {
        b_halo_width_available = (SGLbool) (GLF_GET_DELTA_HALO_WIDTH(par_pr_font, par_ui_font_linewidth) != 0);
    }

    if (par_b_haloing && b_halo_width_available) {
        loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_HALO_WIDTH(par_pr_font, (par_ui_font_linewidth - 2))]);
    }
    else {
        if (par_b_haloing) {
            loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_WIDTH(par_pr_font, (par_ui_font_linewidth - 2))]);
        }
        else {
            loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_WIDTH(par_pr_font, par_ui_font_linewidth)]);
        }
    }

    loc_f_x1 = par_f_x + ((SGLfloat) loc_pr_char_mapping->s_x * par_f_ratio_x);
    loc_f_y1 = par_f_y + ((SGLfloat) loc_pr_char_mapping->s_y * par_f_ratio_y);
    loc_f_x2 = loc_f_x1 + ((SGLfloat) loc_pr_char_mapping->us_width * par_f_ratio_x);
    loc_f_y2 = loc_f_y1 + ((SGLfloat) loc_pr_char_mapping->us_height * par_f_ratio_y);

    oglxSelectFontTexture(loc_pr_char_mapping->ui_map_index);

    /* Read texture coordinates */
    loc_t_tex_coord_array[0][0] = loc_pr_char_mapping->f_u1;
    loc_t_tex_coord_array[0][1] = loc_pr_char_mapping->f_v1;
    loc_t_tex_coord_array[2][0] = loc_pr_char_mapping->f_u2;
    loc_t_tex_coord_array[1][1] = loc_pr_char_mapping->f_v2;
    loc_t_tex_coord_array[1][0] = loc_t_tex_coord_array[0][0];
    loc_t_tex_coord_array[2][1] = loc_t_tex_coord_array[1][1];
    loc_t_tex_coord_array[3][0] = loc_t_tex_coord_array[2][0];
    loc_t_tex_coord_array[3][1] = loc_t_tex_coord_array[0][1];

    /* Fill vertex arrays with a textured rectangle */
    loc_t_vertex_array[0][0] = loc_f_x1;
    loc_t_vertex_array[0][1] = loc_f_y1;
    loc_t_vertex_array[1][0] = loc_f_x1;
    loc_t_vertex_array[1][1] = loc_f_y2;
    loc_t_vertex_array[2][0] = loc_f_x2;
    loc_t_vertex_array[2][1] = loc_f_y2;
    loc_t_vertex_array[3][0] = loc_f_x2;
    loc_t_vertex_array[3][1] = loc_f_y1;


    if (!par_b_haloing) {
        oglxEnable(GL_BLEND);
#ifndef ES2_DEV_ENV
        if (glob_pr_sglStatemachine->b_haloing_state && !b_halo_width_available) {
            glEnable(GL_ALPHA_TEST);
            oglxDrawTexVertexArray(GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
            glDisable(GL_ALPHA_TEST);
        }
        else {
            SGLbyte loc_b_modulate_backup = glob_pr_sglStatemachine->b_modulate;
            if (!loc_b_modulate_backup) {
                if (glob_pr_sglStatemachine->r_current_color.tf_current_color[3] < 1.0F) {
                    sglEnable(SGL_MODULATE);
                }
            }
            oglxDrawTexVertexArray(GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
            if (!loc_b_modulate_backup) {
                sglDisable(SGL_MODULATE);
            }
        }
#else
        oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), SGL_TRUE);
#endif
    }
    else {
        oglxEnable(GL_BLEND);
#ifndef ES2_DEV_ENV
        oglxDrawTexVertexArray(GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
#else
        oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), SGL_TRUE);
#endif
    }

    return;
}

/* End of File ***************************************************************/
