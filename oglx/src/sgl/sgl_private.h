/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_private.h
 DESCRIPTION       : Internal constants, structures and functions of OGLX
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- +*/

#ifndef _SGL_PRIVATE_H
#define _SGL_PRIVATE_H

#include "glf.h"
#include "sgl_context_types.h"

/* NULL constant */
#define SGL_NULL ((void *)0)

/*
**  Halo mode different from SGL_ON and SGL_OFF
**  Only one part of a line is active (Halo or colored part)
*/
#define SGL_HALOED_PART_ONLY				2U
#define SGL_COLORED_PART_ONLY				3U

/* Drawing state active when sglBegin has not been called yet */
#define SGL_NO_VERTEX                       0U

/* Different states of clip planes: 
	SGL_CP_OFF: clip plane not defined, 
	SGL_CP_ENABLE: clip plane active,
	SGL_CP_AVAILABLE: clip plane inactive  
*/
#define SGL_CP_OFF                          0U
#define SGL_CP_ENABLE                       1U
#define SGL_CP_AVAILABLE                    2U

/* Default Ratio used when ratio is not changed by sglViewport */
#define SGL_DEFAULT_RATIO                   1.0F

/* Primitives drawing mode:
	SGL_MODE_UNDEFINED: no drawing mode (to draw textures for example), 
	SGL_MODE_LINE: a line is drawn,
	SGL_MODE_FILL: a filled primitive is drawn 
*/
#define SGL_MODE_UNDEFINED                  0U
#define SGL_MODE_LINE                       1U
#define SGL_MODE_FILL                       2U

/* Different visibility states of a clip box:
	SGL_OGL_STENCIL_UNDEFINED: the clip box is not defined,
	SGL_OGL_VISIBLE_OUTSIDE: visible part is out of the rectangle defining the clip box,
	SGL_OGL_VISIBLE_INSIDE: visible part is inside of the rectangle defining the clip box 
*/
#define SGL_OGL_STENCIL_UNDEFINED           0U
#define SGL_OGL_VISIBLE_OUTSIDE             1U
#define SGL_OGL_VISIBLE_INSIDE              2U

/* There are 18 attributes, but the IDs start from 1, therefore 19 elements.
 The element 0 will never be used   */

/* Mask to handle dirty bits*/
#define LINEWIDTH_MASK			((SGLbyte)0x01)
#define COLOR_INDEX_MASK		((SGLbyte)0x02)
#define STIPPLE_INDEX_MASK		((SGLbyte)0x04)
#define ALPHA_MASK				((SGLbyte)0x08)

/*****************************************************************************
** For glfLoadTagMap
*****************************************************************************/
/* Supported list of tags */
#define GLF_FONT_TAG_ESC    0U
#define GLF_FONT_TAG_CR     1U
#define GLF_FONT_TAG_LF     2U
#define GLF_FONT_TAG_COL    3U
#define GLF_FONT_TAG_LNW    4U
#define GLF_FONT_TAG_FNT    5U
#define GLF_ARGUMENT_0      0x30U

/* ID used when an invalid font index is given */
#define INVALID_FONT_ID			0x0FFFFFFFUL
#define INVALID_TEXTURE_NUMBER	0x0FFFFFFFUL

#define FONT_LINE_WIDTH_SIZE							8U
extern sgl_line_width font_line_width[FONT_LINE_WIDTH_SIZE];

#define DEFAULT_LINE_STIPPLE_NUMBER						5U
extern const sgl_linestipple default_line_stipple_table[DEFAULT_LINE_STIPPLE_NUMBER];

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M18.4, 6, "Rule M18.4 is not applied here as a union between a float and an int allows easy and fast comparison.")
#endif
typedef union {
    SGLfloat f;
    SGLulong l;
} sgl_flt_bin;

typedef struct {
    SGLbyte b_displayed_lines[SGLU_MAX_STRING_LENGTH + 1U];
    SGLlong l_back_color[SGLU_MAX_STRING_LENGTH + 1U];
    SGLlong l_fore_color[SGLU_MAX_STRING_LENGTH + 1U];
    SGLfloat f_crossed_position;
    SGLfloat f_underline_position;
    SGLfloat f_max_height;
    SGLfloat f_max_vert_advance;
} sgl_char_decoration;

SGLushort glob_us_string[SGLU_MAX_TEXT_AREA_STRING_LENGTH + 1U];

typedef struct {
    SGLfloat f_width;
    SGLfloat f_height;
    SGLlong ui_counter;
    SGLulong ui_index;
    SGLlong l_back_color;
    SGLlong l_fore_color;
    SGLbyte b_activated_lines;
    GLuint ui_last_char;
    SGLbyte b_frame_started;
    SGLbyte b_outline_started;
    SGLulong ui_current_font_id;
    const glf_type_font *p_current_font;
    SGLushort us_index;
    const SGLbyte *pb_string;
    SGLfloat f_max_underline;
    SGLfloat f_min_stroke;
    SGLfloat f_max_vert_advance;
    SGLulong ui_param_font_id;
    SGLbyte b_reverse_video;
    SGLushort us_line_number;
    SGLbyte b_cr_started;
    SGLlong l_rev_color;
} sgl_rich_text_attr;

typedef struct {
    SGLfloat f_radiusout;
    SGLfloat f_radiusin;
    SGLfloat f_angle_ok;
    SGLbyte b_clockwise;
    SGLfloat f_startoutx;
    SGLfloat f_startouty;
    SGLfloat f_startinx;
    SGLfloat f_startiny;
    SGLfloat f_centerx;
    SGLfloat f_centery;
    SGLfloat f_angle;
    SGLfloat f_angle_end;
    SGLfloat f_angle_inc;
    SGLlong l_m;
    SGLlong l_n;
} sgl_crown;

typedef struct {
    sgl_line_width *tr_line_width;
    SGLulong ul_nb_line_width;
    SGLlong l_linewidth_index;
    SGLbyte b_polygon_smooth;
    SGLbyte b_stipple_ok;
    SGLbyte b_haloing_state;
    SGLbyte b_type;
} sgl_polygon_smooth_backup;

typedef struct {
    SGLushort us_nb_lines;
    SGLbyte b_start;
    SGLbyte b_end;
    SGLfloat f_x1;
    SGLfloat f_y1;
    SGLfloat f_x2;
    SGLfloat f_y2;
    SGLfloat f_sa_ratio;
    SGLfloat f_ray;
    SGLfloat f_ca_tangent;
    SGLfloat f_sa_tangent;
} sgl_smooth_line;


/* Access to font by index (or default font) */
const glf_type_font *oglx_get_font(SGLulong par_ul_index);

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M8.8.1, 4, "Rule M8.8.1 is not applied here as the declaration is given here and the definition is given in sgl_intern_data.c")
#endif
/* Global structure declaration */
extern sgl_type_statemachine *glob_pr_sglStatemachine;

extern sgl_triangle glob_t_triangles[SGL_MAX_VERTEX_ARRAY_SIZE];
extern SGLulong glob_ul_triangle_number;

/*
** Internal functions prototypes
*/

extern void sgl_backup_for_polygon_smooth(sgl_polygon_smooth_backup * par_p_backup);

extern void sgl_restore_after_polygon_smooth(sgl_polygon_smooth_backup * par_p_backup);

extern void sgl_draw_line(void);

extern void sgl_stencil_buffer_Stencil_update(void);

extern void sgl_vertex_init(SGLfloat par_f_x, SGLfloat par_f_y);

extern void sgl_vertex_none(SGLfloat par_f_x, SGLfloat par_f_y);

extern void sgl_vertex_line(SGLfloat par_f_x, SGLfloat par_f_y);

extern void sgl_vertex_stencil(SGLfloat par_f_x, SGLfloat par_f_y);

extern void sgl_build_vertex_array(SGLfloat par_f_x, SGLfloat par_f_y);

extern void oglxSetError(SGLulong par_ui_error_id, SGLulong par_ui_wrong_parameter);

extern void oglxSelectFont(SGLulong par_ui_font_identifier);

extern void oglxSelectFontTexture(SGLulong par_ui_texture_number);

extern void glf_compute_character_size(SGLulong par_us_index, SGLulong par_ui_counter, SGLfloat * par_pf_width, SGLfloat * par_pf_height);

extern SGLfloat glf_write_attributed_text_multiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                                    SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing,
                                                    SGLfloat par_f_y_delta);

extern SGLfloat glf_draw_stroke_character(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_us_index, SGLbool par_b_haloing);

extern SGLfloat glf_draw_bitmap_character(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_us_index, SGLulong par_ui_font_linewidth, SGLbool par_b_haloing);

extern void glf_draw_bitmap_string(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                   SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing);

extern void glf_draw_stroke_string(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLbool par_b_haloing);

extern SGLfloat glf_draw_bitmap_string_multiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                                 SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing,
                                                 SGLfloat par_f_y_delta);

extern SGLfloat glf_draw_stroke_string_multiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                                 SGLbool par_b_haloing, SGLfloat par_f_y_delta);

extern void glf_write_attributed_text(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                      SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing);

extern SGLushort oglx_get_char_index(GLushort * par_pus_widechar, const GLubyte * par_pub_string);

extern void oglx_fonts_draw_stroke_character(const glf_type_font * par_pr_font, const glf_type_character_stroke * par_pr_char_stroke,
                                             SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y,
                                             SGLulong par_ui_current_font_linewidth_index, SGLulong par_ui_current_font_color_index, SGLbool par_b_haloing);

extern void oglx_fonts_draw_bitmap_character(const glf_type_font * par_pr_font, const glf_type_character_bitmap * par_pr_char,
                                             SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_linewidth,
                                             SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y, SGLbool par_b_haloing);

extern void oglxEnable(SGLulong par_e_Mode);

extern void oglxDisable(SGLulong par_e_Mode);

extern void oglxColor4f(SGLfloat par_f_red, SGLfloat par_f_green, SGLfloat par_f_blue, SGLfloat par_f_alpha);

extern void oglxColor4fv(const SGLfloat * par_pf_color);

extern void oglxUpdateColor(SGLlong par_l_index);

extern void sgl_add_arc_segment(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2);

extern void sglu_arc_sector_2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty, SGLfloat par_f_radius,
                               SGLbool par_b_clockwise, SGLfloat par_f_angle, SGLbyte par_b_as_value);

extern void sgl_draw_filled_arc_sector(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                       SGLfloat par_f_radius, SGLbool par_b_clockwise, SGLfloat par_f_angle);

extern void sgl_draw_outline_arc_sector(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                        SGLfloat par_f_radius, SGLbool par_b_clockwise, SGLfloat par_f_angle);

extern void sgl_draw_circle_line_arc_sector(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty, SGLfloat par_f_radius,
                                            SGLbool par_b_clockwise, SGLfloat par_f_angle);

extern SGLbyte sglu_arc_sector_2f_detect_error(SGLbyte par_b_as_value, SGLbool par_b_clockwise, SGLfloat par_f_start_angle, SGLfloat par_f_end_angle);

extern void sglTexCoord2f(SGLfloat par_f_u, SGLfloat par_f_v);

extern void sgluArcCircleLinePointsList2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                          SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLbyte par_b_clockwise, SGLfloat par_f_angle);

extern void oglxVertex2f(SGLfloat par_f_x, SGLfloat par_f_y);

extern void oglxSetTextureCoordinates(SGLfloat par_f_x, SGLfloat par_f_y);

extern SGLfloat str_get_string_height(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id);

extern SGLfloat str_get_string_width(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id, SGLbool par_b_count_correction);

extern SGLfloat str_get_string_advance(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id, SGLbyte par_b_vertical);

extern SGLbyte sgl_is_word_separator(SGLushort par_us_char);

extern SGLbyte sgl_is_line_separator(SGLushort par_us_char);

extern SGLbool sgl_pixel_point_is_inside_scissors(SGLfloat par_f_x, SGLfloat par_f_y);

extern SGLulong sgl_define_string_lines(SGLushort * par_pus_string, SGLulong par_ul_string_length, SGLfloat par_f_area_width);

extern void sgl_draw_string_lines(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                                  SGLfloat par_f_area_height, SGLulong par_ul_string_length, SGLulong par_ul_font_id, SGLulong par_ul_number_of_lines);

extern void sgl_stencil_buffer_Stencil_end(void);

extern void sgl_end_line(void);

extern void sgl_end_shape(void);

extern void sgl_set_line_mode(void);

extern void sgl_set_fill_mode(void);

extern void sgl_stencil_buffer_Stencil_begin(SGLulong par_ul_stencilID);

extern void sgl_compute_nx_ny(SGLfloat par_f_horiz_pattern, SGLfloat par_f_vert_pattern);

extern void sgl_compute_dx_dy(SGLlong par_l_horiz_align, SGLlong par_l_vert_align);

extern SGLbyte sgl_format_texture_detect_error(SGLulong par_ul_nb_vertices, SGLfloat * par_pf_x, SGLfloat * par_pf_y,
                                               SGLlong par_l_horiz_align, SGLlong par_l_vert_align);

extern SGLbool sgl_get_font_value_detect_error(SGLulong par_ul_font_id, SGLbyte par_b_value_kind);

extern SGLbool sglu_arc_segment_detect_error(SGLbool par_b_clockwise, SGLfloat par_f_angle);

extern SGLbool sgl_pixel_rectangle_is_inside_scissors(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2);

extern void sgl_stencil_buffer_ClipBox_description(SGLulong par_ul_clipboxID);

extern SGLbyte sgl_clip_box_detect_error(SGLlong par_l_number, SGLbyte par_b_type, SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left,
                                         SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right);

extern void sgl_clear_opengl_stencil(void);

extern void sgl_disable_stencil_test(void);

extern SGLulong oglx_load_texture(SGLlong par_l_width, SGLlong par_l_height);

#ifndef OGLX_NO_GLCLIPPLANE
extern void sgl_initialize_clip_plane_structure(void);

extern void sgl_clip_plane(SGLlong par_l_number, const SGLfloat * par_pf_data);

extern void sgl_enable_clip_plane(SGLulong par_ul_cap);

extern void sgl_disable_clip_plane(SGLulong par_ul_cap);
#endif

extern void sgl_enable_stencil(SGLlong par_l_stencil_id);

extern void sgl_disable_stencil(SGLlong par_l_stencil_id);

extern void sgl_enable_clip_box(SGLlong par_l_clip_box_id);

extern void sgl_disable_clip_box(SGLlong par_l_clip_box_id);

#ifndef OGLX_NO_GLCLIPPLANE
extern void sgl_disable_clip_planes(void);
#endif

extern void sgl_set_texture_parameters(SGLbyte par_b_texture_application_mode);

extern void sgl_set_font_texture_parameters(void);

extern void sgl_draw_arrays_gl_lines(void);

extern void sgl_draw_smooth_single_line(sgl_smooth_line * par_pr_smooth_line);

extern void sgl_draw_simple_line(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2);

extern void sgl_draw_raw_lines(void);

extern void sgl_draw_raw_lines_loop_strip(void);

extern void sgl_draw_smooth_lines(SGLfloat par_f_half_pixel_width);

extern void sgl_draw_continuous_lines(SGLfloat par_f_alpha_halo, SGLfloat par_f_half_pixel_width_haloed, SGLfloat par_f_half_pixel_width,
                                      SGLbyte par_b_haloed_part, SGLbyte par_b_colored_part);

extern void sgl_set_line_width(SGLfloat par_f_line_width);

extern SGLulong sgl_vertex_2f_line_stipple(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2, SGLulong par_ul_index);

extern SGLbool sgl_vertex_2f_add_line(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2);

extern SGLulong sgl_define_lines(SGLushort * par_pus_string, SGLulong par_ul_string_length);

extern void sgl_stencil_compute_union_clipboxes(void);

extern void sgl_define_stencils_mask(SGLulong par_ul_mask);

extern void sgl_define_cb_and_stencil_mask(SGLulong par_ul_mask);

extern void sgl_define_cb1_and_both_stencil_mask(SGLulong par_ul_mask);

extern void sgl_define_cb2_and_both_stencil_mask(SGLulong par_ul_mask);

extern void sgl_define_both_clipbox_mask(SGLulong par_ul_mask);

extern void sgl_define_both_clipbox_and_stencil_mask(SGLulong par_ul_mask);

extern void sgl_compute_scissor_intersection(const scissor_definition * par_p_old_scissor, scissor_definition * par_p_new_scissor);

extern SGLbyte oglxUncompressBitmap(SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_pixel_array_size,
                                    const SGLulong * par_pul_pixel);


extern void oglxEnlargeTexture(SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_enlarged_width, SGLlong par_l_enlarged_height,
                               SGLlong par_l_component_number);

extern void oglxEnlargeTextureKeepDimensions(SGLlong par_l_width, SGLlong par_l_height, SGLlong par_l_enlarged_width, SGLlong par_l_enlarged_height,
                                             SGLlong par_l_component_number);

extern void sgl_tex_image_2D_ubv_uncompressed(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height,
                                              SGLbyte par_b_texture_application_mode);
extern SGLbyte sgl_comp_teximage2d_detect_error(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height,
                                                const SGLulong * par_pul_pixel, SGLbyte par_b_texture_application_mode);

extern void sgl_initialize_clipping(void);

extern void sgl_initialize_resources_tables(void);

extern void sgl_initialize_appearance_attributes(void);

extern void sgl_initialize_vertex_data(void);

extern void sgl_initialize_texture_data(void);

extern void sgl_initialize_transformation(void);

extern SGLbool sgl_is_power_of_two(SGLulong par_ul_value);

extern SGLbool sgl_init_detect_error(sgl_type_statemachine * par_s_context, SGLulong par_ul_screen_width, SGLulong par_ul_screen_height,
                                     const SGLbyte * par_pb_texture_buffer, SGLulong par_ul_texture_max_width, SGLulong par_ul_texture_max_height);

extern void sgl_reset_text_attributes(void);

extern void sgl_reset_appearance_attributes(void);

extern void sgl_reset_clipping_attributes(void);

extern void sgl_reset_mode_attributes(void);

extern SGLbyte sgl_teximage_2dubv_detect_error(SGLlong par_l_texture_number, SGLbyte par_b_format, SGLlong par_l_width, SGLlong par_l_height,
                                               const SGLbyte * par_pb_color, SGLbyte par_b_texture_application_mode);


extern SGLfloat oglx_min_value(SGLfloat par_f_value1, SGLfloat par_f_value2);

extern SGLfloat oglx_max_value(SGLfloat par_f_value1, SGLfloat par_f_value2);

extern void oglxTexVertex4f(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_u, SGLfloat par_f_v);

extern void ogl_simple_arc(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx,
                           SGLfloat par_f_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius,
                           SGLbool par_b_duplicate);

extern void ogl_stippled_arc(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx,
                             SGLfloat par_f_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius,
                             SGLfloat * par_f_last_x, SGLfloat * par_f_last_y);

extern SGLfloat ogl_sglu_compute_arc_distance(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty);

extern void ogl_sglu_arc_line_loop_strip(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                         SGLfloat par_f_calculated_startx, SGLfloat par_f_calculated_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok,
                                         SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLfloat * par_f_last_pointx, SGLfloat * par_f_last_pointy);

extern void ogl_sglu_arc_lines(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                               SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius,
                               SGLfloat * par_f_last_pointx, SGLfloat * par_f_last_pointy);

extern void sgl_draw_filled_crown(const sgl_crown * par_s_crown);

extern SGLbyte sglu_crown_2f_detect_error(SGLfloat par_f_width, SGLfloat par_f_radius, SGLbyte par_b_crown_value, SGLbyte par_b_clockwise);

extern void sglu_crown_2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_radius, SGLfloat par_f_startinx, SGLfloat par_f_startiny,
                          SGLfloat par_f_width, SGLbyte par_b_clockwise, SGLfloat par_f_angle_in, SGLbyte par_b_crown_value);

extern void sgl_draw_simple_outline_crown(const sgl_crown * par_s_crown);

extern sgl_bounding_box glf_get_rich_text_bounds(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string,
                                                 SGLulong par_ui_string_length, SGLfloat par_f_hor_alignment, SGLfloat par_f_vert_alignment);

extern sgl_bounding_box glfGetRichTextBounds(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, const SGLbyte * par_pb_string,
                                             SGLulong par_ui_string_length, SGLfloat par_f_hor_alignment, SGLfloat par_f_vert_alignment);


extern void sgl_separate_1st_2nd_font(SGLbyte * par_b_left_part, SGLlong par_l_integer_length,
                                      SGLbyte * par_b_right_part, SGLlong par_l_fractional_length, sgl_numeric_format * par_p_format, SGLlong par_l_sign);

extern void sgl_add_leading_zeroes(SGLbyte * par_b_string, SGLlong * par_l_integer_length, SGLlong par_l_format_length);

extern void sgl_truncate_integer_part(SGLbyte * par_b_left_part, SGLlong par_l_integer_length, SGLlong par_l_integral_part);

extern void sgl_add_sign_character(SGLbyte * par_b_left_part, SGLlong par_l_integer_length, SGLlong par_l_integral_part, SGLbyte par_b_sign);

extern void sgl_change_font_integral_part(SGLbyte * par_b_left_part, SGLlong par_l_integer_length,
                                          SGLbyte * par_b_right_part, SGLlong par_l_fractional_length, sgl_numeric_format * par_p_format);

extern void sgl_change_font_fractional_part(SGLbyte * par_b_left_part, SGLlong par_l_integer_length,
                                            SGLbyte * par_b_right_part, sgl_numeric_format * par_p_format);


extern SGLbool sglu_write_float_detected_errors(sgl_numeric_format * par_p_format);

extern void glf_write_attributed_rich_text(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLulong par_ui_string_length,
                                           SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color);

extern void glfWriteAttributedComplexText(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, const SGLbyte * par_pb_string,
                                          SGLulong par_ui_string_length);

extern void ogl_sgluWriteRichText(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLulong par_l_string_length);

extern void oglx_draw_character_decoration(sgl_char_decoration * par_char_decoration, SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_index,
                                           SGLfloat par_f_horiz_advance);

extern void oglx_draw_character_background(sgl_char_decoration * par_char_decoration, SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_index,
                                           SGLfloat par_f_horiz_advance);

extern void oglx_char_decoration(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLulong par_ui_string_length);

extern void oglx_change_font(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index);

extern void oglx_change_forecolor_deco(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index);

extern void oglx_change_backcolor_deco(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index);

extern void oglx_change_outline_deco(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLulong par_us_index);

extern void oglx_define_characters_decoration(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLfloat par_f_x,
                                              const SGLbyte * par_pb_string, SGLulong par_ui_string_length);

extern void oglx_draw_character(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLfloat * par_f_x, SGLfloat par_f_y,
                                SGLfloat par_f_tx, SGLulong par_ui_font_linewidth);

extern void oglx_change_fore_color(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index);

extern void oglx_video_inv(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration);

extern void oglx_draw_rich_text(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLfloat par_f_x, SGLfloat par_f_y,
                                const SGLbyte * par_pb_string, SGLulong par_ui_string_length, SGLulong par_ui_font_linewidth);

extern void oglx_compute_character_bounds(sgl_rich_text_attr * par_rich_text_attr);

extern void glfWriteAttributedText(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, SGLushort * par_pus_string,
                                   SGLulong par_ui_string_length);

extern void ogl_sgluWriteText(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_l_string_length);

extern void sgl_draw_text_lines_up_to_down(SGLfloat par_f_x_origin, SGLfloat par_f_y_origin, SGLfloat par_f_y_delta, SGLulong par_ul_number_of_lines);

extern void sgl_draw_text_lines_down_to_up(SGLfloat par_f_x_origin, SGLfloat par_f_y_origin, SGLfloat par_f_y_delta, SGLulong par_ul_number_of_lines);

extern void glfWriteAttributedTextMultiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, SGLushort * par_pus_string,
                                            SGLulong par_ui_string_length);

extern void ogl_sgluWriteTextMultiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_l_string_length);


extern void sgl_draw_lines_up_to_down(SGLfloat par_f_x_origin, SGLfloat par_f_y_origin, SGLfloat par_f_y_delta,
                                      SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing,
                                      SGLulong par_ul_number_of_lines);

extern void glf_draw_aligned_lines(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_char_height, SGLfloat par_f_v_advance,
                                   SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing, SGLulong par_ul_number_of_lines);

extern SGLulong oglx_get_angle_position(SGLfloat par_f_angle);

extern SGLbool sgl_get_arc_segment_bounds_detect_error(SGLbool par_b_clockwise, SGLfloat par_f_angle);

extern SGLfloat ogl_compute_factor(SGLfloat par_f_a, SGLfloat par_f_b, SGLfloat par_f_angle);

extern void oglxBindTexture(GLuint par_ui_index);

extern void sgl_close_line_loop(void);

extern void sgl_draw_shape(void);

extern void sgl_finish_shape_arc(void);

extern void sgl_draw_stencil(void);

extern SGLbyte sglu_arc_ellipse_2f_detect_error(SGLfloat par_f_x_radius, SGLfloat par_f_y_radius);

extern SGLbool sgl_point_in_polygon(SGLulong par_ul_nb_vertices, sgl_vector2d * par_p_vertices, SGLfloat par_f_x, SGLfloat par_f_y);

extern SGLbool sgl_is_ear(SGLulong par_ul_ear_index);

extern SGLulong sgl_get_first_ear(void);

extern SGLulong sgl_add_triangle_suppress_ear(SGLulong par_ul_ear_index, SGLbool par_b_texture);

extern void sgl_draw_triangles(SGLbool par_b_texture);

extern void sgl_tessellation(void);

extern void oglxTranslateZ(SGLfloat par_f_z);

extern void oglxTexEnvi(SGLulong par_ul_target, SGLulong par_ul_pname, SGLlong par_l_param);

extern sgl_bounding_box oglxComputeTextAreaBounds(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right,
                                                  SGLfloat par_f_y_top_right, SGLlong par_l_string_length, SGLlong par_l_font_id, SGLlong par_l_vert_alignment);

extern sgl_bounding_box oglxComputeTextBounds(SGLfloat par_f_x, SGLfloat par_f_y,
                                              SGLlong par_l_string_length, SGLlong par_l_font_id, SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment);

extern void oglxGetEntirePixelCoordinate(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_x, SGLfloat * par_pf_y);

extern sgl_line_string ts_lines[SGL_MAX_STRING_LINES];

#ifdef ES2_DEV_ENV

#define OGLX_ES2_VERTEX_ARRAY		0
#define OGLX_ES2_COLOR				1
#define OGLX_ES2_TEX_COORD_ARRAY	2
#define OGLX_ES2_TEXTURE_ACTIVE		3

extern void oglxLoadMatrixf(const SGLfloat * par_pf_matrix);
extern void oglxPopMatrix(void);
extern void oglxPushMatrix(void);
extern void oglxDrawTexVertexArray_es2(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices, SGLbyte par_uc_apply_color);
extern void oglxDrawVertexArray_es2(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices);

extern SGLbool sglReset_es2(void);

extern SGLbyte sglInit_es2();

extern void sglTerminate_es2(void);

extern void oglxSetTextureState(int par_i_state);

extern void oglxUpdateColor_es2(const SGLfloat * par_pf_color);
#else

/* Table functions */
extern void oglxDrawVertexArray(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices);
extern void oglxDrawTexVertexArray(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices, const SGLfloat * par_pf_texcoord);

#endif

#ifndef OGLX_NO_GLCLIPPLANE
extern SGLbyte sgl_get_clip_plane_number(SGLlong par_l_number);
#endif

#endif                          /* _SGL_PRIVATE_H */
