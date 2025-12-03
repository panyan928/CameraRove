/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxReadErrors.c
 DESCRIPTION       : oglxReadErrors shall provide a readable strucure for errors storage.
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl_context_types.h"
#include "oglxReadErrors.h"

extern sgl_type_statemachine *glob_pr_sglStatemachine;

oglx_error glob_s_error_table[SGL_ERROR_MAX];

#define OGL_ERRORS_NUMBER 129UL

oglx_error_definition glob_errors_definition[OGL_ERRORS_NUMBER] = {
    /* 0: No Error */
    {"", "", ""},

    /* 1: SGL_ERROR_SGL_BEGIN */
    {
     "SGL_ERROR_SGL_BEGIN",
     "Parameter par_b_mode is different from SGL_LINES, SGL_LINE_STRIP, SGL_LINE_LOOP, SGL_POLYGON, SGL_TRIANGLE_STRIP, SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8, SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128.",
     "par_b_mode"},

    /* 2: SGL_ERROR_SGL_BINDTEXTURE */
    {
     "SGL_ERROR_SGL_BINDTEXTURE",
     "One of following:\n\
- Parameter par_l_texture_number is out of range [0,<number of textures>-1].\n\
- Parameter par_l_texture_number specifies a texture that is not defined.\n\
- Parameter par_b_target is different from SGL_TEXTURE_2D.\n\
- The attribute p_texture_attrib given to sglInit is SGL_NULL.",
     "par_l_texture_number or par_b_target"},

    /* 3: No Error */
    {"", "", ""},

    /* 4: SGL_ERROR_SGL_CLIPBOX */
    {
     "SGL_ERROR_SGL_CLIPBOX",
     "One of following:\n\
- Parameter par_ul_number is different from SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8, SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128.\n\
- Parameter par_b_type is different from SGL_VISIBLE_INSIDE, SGL_VISIBLE_OUTSIDE.\n\
- Difference between par_f_x_top_right and par_f_x_bottom_left is close to zero.\n\
- Difference between par_f_y_top_right and par_f_y_bottom_left is close to zero.",
     "par_ul_number"},

    /* 5: SGL_ERROR_SGL_CLIPPLANE */
    {
     "SGL_ERROR_SGL_CLIPPLANE",
     "Parameter par_ul_number is different from SGL_CLIP_PLANE0, SGL_CLIP_PLANE1, SGL_CLIP_PLANE2, SGL_CLIP_PLANE3, SGL_CLIP_PLANE4, SGL_CLIP_PLANE5.",
     "par_ul_number"},

    /* 6: SGL_ERROR_SGL_ENABLE */
    {
     "SGL_ERROR_SGL_ENABLE",
     "Parameter par_b_cap is different from SGL_POLYGON_SMOOTH, SGL_LINE_HALOING, SGL_TEXTURE_2D, SGL_MODULATE, SGL_HALO_SAME_LEVEL, SGL_TESSELLATION, SGL_TEXT_POS_ADJUSTMENT, SGL_GRADIENT.",
     "par_b_cap"},

    /* 7: SGL_ERROR_SGL_DISABLE */
    {
     "SGL_ERROR_SGL_DISABLE",
     "Parameter par_b_cap is different from SGL_POLYGON_SMOOTH, SGL_LINE_HALOING, SGL_TEXTURE_2D, SGL_MODULATE, SGL_HALO_SAME_LEVEL, SGL_TESSELLATION, SGL_TEXT_POS_ADJUSTMENT, SGL_GRADIENT.",
     "par_b_cap"},

    /* 8: No Error */
    {"", "", ""},

    /* 9: SGL_ERROR_SGLU_LOADFONTS */
    {
     "SGL_ERROR_SGLU_LOADFONTS",
     "One of following:\n\
1) The input font format is not compatible with OGLX version.\n\
2) The input font contains a bitmap with a width (resp. height) greater than ul_texture_max_width (resp. ul_texture_max_height) of sglInit",
     "1) or 2)"},

    /* 10: No Error */
    {"", "", ""},

    /* 11: No Error */
    {"", "", ""},

    /* 12: No Error */
    {"", "", ""},

    /* 13: SGL_ERROR_SGL_ORTHO */
    {
     "SGL_ERROR_SGL_ORTHO",
     "One of following:\n\
- The modelview matrix is not empty.\n\
- Difference of par_f_right and par_f_left is close to zero.\n\
- Difference of par_f_top and par_f_bottom is close to zero.",
     "N/A"},

    /* 14: No Error */
    {"", "", ""},

    /* 15: SGL_ERROR_SGL_PUSHMATRIX */
    {
     "SGL_ERROR_SGL_PUSHMATRIX",
     "The modelview matrix is full.",
     "N/A"},

    /* 16: SGL_ERROR_SGL_POPMATRIX */
    {
     "SGL_ERROR_SGL_POPMATRIX",
     "The modelview matrix is empty.",
     "N/A"},

    /* 17: No Error */
    {"", "", ""},

    /* 18: No Error */
    {"", "", ""},

    /* 19: No Error */
    {"", "", ""},

    /* 20: SGL_ERROR_SGL_VERTEX2F */
    {
     "SGL_ERROR_SGL_VERTEX2F",
     "The total number of vertices is greater than SGL_MAX_VERTEX_ARRAY_SIZE.",
     "N/A"},

    /* 21: No Error */
    {"", "", ""},

    /* 22: No Error */
    {"", "", ""},

    /* 23: No Error */
    {"", "", ""},

    /* 24: No Error */
    {"", "", ""},

    /* 25: No Error */
    {"", "", ""},

    /* 26: SGL_ERROR_SGLU_ARCSECTOR2F */
    {
     "SGL_ERROR_SGLU_ARCSECTOR2F",
     "One of following:\n\
- Parameter par_b_clockwise is different from SGL_TRUE and SGL_FALSE.\n\
- Parameter par_b_as_value is different from SGL_ARC_FILLED, SGL_ARC_CIRCLE_LINE, SGL_ARC_OUTLINE.\n\
- Parameter par_f_start_angle or par_f_end_angle are out of range [-99999, 99999].",
     "N/A"},

    /* 27: SGL_ERROR_SGLU_CROWN2F */
    {
     "SGL_ERROR_SGLU_CROWN2F",
     "One of following:\n\
- Parameter par_f_width is not strictly positive.\n\
- Parameter par_f_width is greater than par_f_radius.\n\
- Parameter par_b_clockwise is different from SGL_TRUE, SGL_FALSE.\n\
- Parameter par_b_crown_value is different from SGL_CROWN_FILLED, SGL_CROWN_OUTLINE.",
     "N/A"},

    /* 28: No Error */
    {"", "", ""},

    /* 29: SGL_ERROR_SGL_INDEXFONTI */
    {
     "SGL_ERROR_SGL_INDEXFONTI",
     "Parameter par_l_font_id is out of range [0,<number of fonts>-1].",
     "par_l_font_id"},

    /* 30: No Error */
    {"", "", ""},

    /* 31: SGL_ERROR_SGLU_WRITETEXT */
    {
     "SGL_ERROR_SGLU_WRITETEXT",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_STRING_LENGTH].\n\
- Parameter par_pb_string is SGL_NULL (error argument set to 0).",
     "par_l_string_length or 0"},

    /* 32: SGL_ERROR_SGL_VIEWPORT */
    {
     "SGL_ERROR_SGL_VIEWPORT",
     "One of following:\n\
- The modelview matrix is not empty.\n\
- Parameter par_l_width is lower than 1.\n\
- Parameter par_l_height is lower than 1.",
     "N/A"},

    /* 33: No Error */
    {"", "", ""},
    /* 34: No Error */
    {"", "", ""},
    /* 35: No Error */
    {"", "", ""},
    /* 36: No Error */
    {"", "", ""},
    /* 37: No Error */
    {"", "", ""},
    /* 38: No Error */
    {"", "", ""},
    /* 39: No Error */
    {"", "", ""},
    /* 40: No Error */
    {"", "", ""},
    /* 41: No Error */
    {"", "", ""},
    /* 42: No Error */
    {"", "", ""},
    /* 43: No Error */
    {"", "", ""},
    /* 44: No Error */
    {"", "", ""},
    /* 45: No Error */
    {"", "", ""},
    /* 46: No Error */
    {"", "", ""},
    /* 47: No Error */
    {"", "", ""},
    /* 48: No Error */
    {"", "", ""},

    /* 49: SGL_ERROR_SGL_INDEXCOLORI */
    {
     "SGL_ERROR_SGL_INDEXCOLORI",
     "Parameter par_l_index is out of range [0,<number of colors>-1].",
     "par_l_index"},

    /* 50: SGL_ERROR_SGL_INDEXLINESTIPPLE */
    {
     "SGL_ERROR_SGL_INDEXLINESTIPPLE",
     "Parameter par_l_index is out of range [0,<number of line stipples>-1].",
     "par_l_index"},

    /* 51: SGL_ERROR_SGL_INDEXLINEWIDTHI */
    {
     "SGL_ERROR_SGL_INDEXLINEWIDTHI",
     "Parameter par_l_index is out of range [0,<number of line widths>-1].",
     "par_l_index"},

    /* 52: SGL_ERROR_SGL_TEXIMAGE2DUBV */
    {
     "SGL_ERROR_SGL_TEXIMAGE2DUBV",
     "One of following:\n\
- The texture mode is enabled and the specified texture is bound.\n\
- Parameter par_l_texture_number is out of range [0, <ul_number_of_textures of sglInit>-1].\n\
- Parameter par_l_width is out of range [1, <par_ul_texture_max_width of sglInit>].\n\
- Parameter par_l_height is out of range [1, <par_ul_texture_max_height of sglInit>].\n\
- Parameter par_b_format is different from SGL_BITMAP_RGB_NOT_TRANSPARENT, SGL_BITMAP_RGB_TRANSPARENT, SGL_BITMAP_ALPHA.\n\
- The texture buffer given to sglInit is SGL_NULL.\n\
- Parameter par_b_texture_application_mode is different from SGL_CLAMP, SGL_REPEAT.",
     "par_l_texture_number"},

    /* 53: SGL_ERROR_SGL_COMPRESSEDTEXIMAGE2D */
    {
     "SGL_ERROR_SGL_COMPRESSEDTEXIMAGE2D",
     "One of following:\n\
- The texture mode is enabled and the specified texture is bound.\n\
- Parameter par_l_texture_number is out of range [0, <ul_number_of_textures of sglInit>-1],\n\
- Parameter par_l_width is out of range [1, <par_ul_texture_max_width of sglInit>].\n\
- Parameter par_l_height is out of range [1, <par_ul_texture_max_height of sglInit>].\n\
- Parameter par_b_format is different from SGL_BITMAP_RGB_NOT_TRANSPARENT, SGL_BITMAP_RGB_TRANSPARENT, SGL_BITMAP_ALPHA.\n\
- After decompression, the size of the obtained data is not equal to par_l_width*par_l_height.\n\
- The texture buffer given to sglInit is SGL_NULL.\n\
- Parameter par_b_texture_application_mode is different from SGL_CLAMP, SGL_REPEAT.",
     "par_l_texture_number"},

    /* 54: No Error */
    {"", "", ""},

    /* 55: SGL_ERROR_SGL_TRANSPARENCY8 */
    {
     "SGL_ERROR_SGL_TRANSPARENCY8",
     "Parameter par_l_alpha is not in range [0,255].",
     "par_l_alpha"},

    /* 56: No Error */
    {"", "", ""},

    /* 57: SGL_ERROR_SGL_COLORPOINTERF */
    {
     "SGL_ERROR_SGL_COLORPOINTERF",
     "One of following:\n\
- Parameter par_ul_nb_colors is zero.\n\
- Parameter par_ps_colormap is SGL_NULL.",
     "N/A"},

    /* 58: No Error */
    {"", "", ""},

    /* 59: No Error */
    {"", "", ""},

    /* 60: No Error */
    {"", "", ""},

    /* 61: SGL_ERROR_SGL_LINESTIPPLEPOINTER */
    {
     "SGL_ERROR_SGL_LINESTIPPLEPOINTER",
     "One of following:\n\
- Parameter par_ul_nb_line_stipple is zero.\n\
- Parameter par_ps_linestipple is SGL_NULL.\n\
- One of the line stipple entry has a b_number value greater than 6.",
     "N/A"},

    /* 62: No Error */
    {"", "", ""},
    /* 63: No Error */
    {"", "", ""},
    /* 64: No Error */
    {"", "", ""},

    /* 65: SGL_ERROR_SGL_SETTEXTALIGNMENT */
    {
     "SGL_ERROR_SGL_SETTEXTALIGNMENT",
     "One of following:\n\
- Parameter par_l_hor_alignment is different from SGL_ALIGN_LEFT, SGL_ALIGN_CENTER, SGL_ALIGN_RIGHT.\n\
- Parameter par_l_vert_alignment is different from SGL_ALIGN_BOTTOM, SGL_ALIGN_MIDDLE, SGL_ALIGN_TOP.",
     "N/A"},

    /* 66: SGL_ERROR_SGL_SETACTIVEMASKS */
    {
     "SGL_ERROR_SGL_SETACTIVEMASKS",
     "Parameter par_ul_mask_combination contains a 1-bit different from SGL_CLIP_PLANE0, SGL_CLIP_PLANE1, SGL_CLIP_PLANE2, SGL_CLIP_PLANE3, SGL_CLIP_PLANE4, SGL_CLIP_PLANE5, SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8, SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128.",
     "par_ul_mask_combination"},

    /* 67: SGL_ERROR_SGL_BEGINSCISSOR */
    {
     "SGL_ERROR_SGL_BEGINSCISSOR",
     "sglBeginScissor has been called more than SGL_MAX_SCISSORS times without a call to sglEndScissor.",
     "SGL_MAX_SCISSORS"},

    /* 68: SGL_ERROR_SGL_ENDSCISSOR */
    {
     "SGL_ERROR_SGL_ENDSCISSOR",
     "There is no active scissor when calling sglEndScissor.",
     "N/A"},

    /* 69: SGL_ERROR_SGL_SETWORDSSEPARATORS */
    {
     "SGL_ERROR_SGL_SETWORDSSEPARATORS",
     "One of following:\n\
- Parameter par_ul_nb_separators is greater than SGL_MAX_SEPARATORS.\n\
- Parameter par_pb_separators is SGL_NULL.",
     "par_ul_nb_separators"},

    /* 70: SGL_ERROR_SGL_SETLINESSEPARATORS */
    {
     "SGL_ERROR_SGL_SETLINESSEPARATORS",
     "One of following:\n\
- Parameter par_ul_nb_separators is greater than SGL_MAX_SEPARATORS.\n\
- Parameter par_pb_separators is SGL_NULL.",
     "par_ul_nb_separators"},

    /* 71: SGL_ERROR_SGLU_WRITETEXTAREA */
    {
     "SGL_ERROR_SGLU_WRITETEXTAREA",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_TEXT_AREA_STRING_LENGTH].\n\
- Parameter par_pb_string is SGL_NULL (error agument set to 0).",
     "par_l_string_length or 0"},

    /* 72: SGL_ERROR_SGLU_GETTABLEINDEX */
    {
     "SGL_ERROR_SGLU_GETTABLEINDEX",
     "One of following:\n\
- Parameter par_l_size is negative.\n\
- Parameter par_l_index is not in range [0,par_l_size-1].",
     "par_l_index"},

    /* 73: SGL_ERROR_SGLU_LONGDIV */
    {
     "SGL_ERROR_SGLU_LONGDIV",
     "One of following:\n\
- Parameter par_l_long_2 value is 0.\n\
- Parameter par_l_long_1 value is (-2147483647 - 1) and parameter par_l_long_2 value is -1.",
     "par_l_long_2"},

    /* 74: SGL_ERROR_SGLU_FLOATDIV */
    {
     "SGL_ERROR_SGLU_FLOATDIV",
     "Parameter par_f_float_2 value is 0.0F with a precision of SGL_MIN_F.",
     "N/A"},

    /* 75: SGL_ERROR_SGLU_WRITEFLOAT */
    {
     "SGL_ERROR_SGLU_WRITEFLOAT",
     "One of following:\n\
- Parameter par_p_format->l_integralPart is negative.\n\
- Parameter par_p_format->l_fractionalPart is negative.\n\
- Parameter par_p_format->l_integralPart is strictly greater than 9.\n\
- Parameter par_p_format->l_fractionalPart is strictly greater than 9.\n\
- Parameter par_p_format->l_secondFontPos is out of range [-l_integralPart,l_fractionalPart+1].\n\
- Parameter par_p_format->l_displaySign is different from SGL_SIGN_WHEN_NEGATIVE, SGL_SIGN_ALWAYS, SGL_SIGN_NEVER.\n\
- par_p_format is SGL_NULL",
     "N/A"},

    /* 76: SGL_ERROR_SGLU_DRAWBITMAP */
    {
     "SGL_ERROR_SGLU_DRAWBITMAP",
     "Parameter par_l_texture_number is out of range [0L, <ul_number_of_textures of sglInit>-1].",
     "par_l_texture_number"},

    /* 77: SGL_ERROR_SGLU_WRITERICHTEXT */
    {
     "SGL_ERROR_SGLU_WRITERICHTEXT",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_STRING_LENGTH].\n\
- Parameter par_pb_string is SGL_NULL (error argument set to 0).",
     "par_l_string_length or 0"},

    /* 78: SGL_ERROR_SGL_FORMATTEXTURE */
    {
     "SGL_ERROR_SGL_FORMATTEXTURE",
     "One of following:\n\
- Parameter par_ul_nb_vertices is out of range [2,SGL_MAX_VERTEX_ARRAY_SIZE-1].\n\
- Parameter par_l_horiz_align is different from SGL_ALIGN_LEFT, SGL_ALIGN_RIGHT, SGL_ALIGN_CENTER.\n\
- Parameter par_l_vert_align is different from SGL_ALIGN_BOTTOM, SGL_ALIGN_TOP, SGL_ALIGN_MIDDLE.\n\
- Parameter par_pf_x is SGL_NULL.\n\
- Parameter par_pf_y is SGL_NULL.\n\
- The attribute p_texture_attrib given to sglInit is SGL_NULL.",
     "par_ul_nb_vertices"},

    /* 79: SGL_ERROR_SGL_ALLOCATEMASKID */
    {
     "SGL_ERROR_SGL_ALLOCATEMASKID",
     "Parameter par_b_mask_kind is different from SGL_TYPE_CLIP_PLANE, SGL_TYPE_CLIP_BOX, SGL_TYPE_STENCIL.",
     "N/A"},

    /* 80: SGL_ERROR_SGL_TEXVERTEX4F */
    {
     "SGL_ERROR_SGL_TEXVERTEX4F",
     "One of following:\n\
- SGL_MAX_VERTEX_ARRAY_SIZE vertices have been already specified since last sglBegin.\n\
- Textures are not enable (sglEnable(SGL_TEXTURE_2D) has not been called before).",
     "N/A"},

    /* 81: No Error */
    {"", "", ""},

    /* 82: No Error */
    {"", "", ""},

    /* 83: No Error */
    {"", "", ""},

    /* 84: SGL_ERROR_SGL_SETCURRENTCONTEXT */
    {
     "SGL_ERROR_SGL_SETCURRENTCONTEXT",
     "Parameter par_s_context is SGL_NULL.",
     "N/A"},

    /* 85: SGL_ERROR_SGLU_ARCSEGMENT */
    {
     "SGL_ERROR_SGLU_ARCSEGMENT",
     "One of following:\n\
- Parameter par_b_clockwise is different from SGL_TRUE, SGL_FALSE.\n\
- Parameter par_f_angle is a multiple of 360.0 (including 0.0).",
     "N/A"},

    /* 86: SGL_ERROR_SGL_FREEMASKID */
    {
     "SGL_ERROR_SGL_FREEMASKID",
     "Parameter par_ul_mask_id contains a 1-bit different from SGL_CLIP_PLANE0, SGL_CLIP_PLANE1, SGL_CLIP_PLANE2, SGL_CLIP_PLANE3, SGL_CLIP_PLANE4, SGL_CLIP_PLANE5, SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8, SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128.",
     "par_ul_mask_id"},

    /* 87: SGL_ERROR_SGLU_WRITEUNICODE_TEXTAREA */
    {
     "SGL_ERROR_SGLU_WRITEUNICODE_TEXTAREA",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_TEXT_AREA_STRING_LENGTH].\n\
- Parameter par_pl_string is SGL_NULL (error agument set to 0).\n\
- Parameter par_pl_string contains a negative value before index par_l_string_length (error agument set to 0).",
     "par_l_string_length or 0"},

    /* 88: SGL_ERROR_SGL_GETFONTVALUE */
    {
     "SGL_ERROR_SGL_GETFONTVALUE",
     "One of following:\n\
- Parameter par_b_value_kind is different from SGL_CHAR_WIDTH, SGL_CHAR_HEIGHT, SGL_VERTICAL_ADVANCE, SGL_HORIZONTAL_ADVANCE, SGL_UNDERLINE, SGL_STROKE, SGL_MAX_CHAR_WIDTH, SGL_MAX_CHAR_HEIGHT, SGL_MAX_CHAR_H_ADVANCE, SGL_MAX_CHAR_V_ADVANCE.\n\
- Parameter par_ul_font_id is out of range [0,<number of fonts>-1].\n\
- Parameter par_ul_char_id is not a character present in the font defined by par_ul_font_id.",
     "par_b_value_kind"},

    /* 89: SGL_ERROR_SGLU_MODULOANGLE */
    {
     "SGL_ERROR_SGLU_MODULOANGLE",
     "par_f_angle is out of range [-SGL_VALUE_MAX, SGL_VALUE_MAX].",
     "N/A"},

    /* 90: SGL_ERROR_SGL_GETARCSEGMENTBOUNDS */
    {
     "SGL_ERROR_SGL_GETARCSEGMENTBOUNDS",
     "One of following:\n\
- Parameter par_b_clockwise is different from SGL_TRUE, SGL_FALSE.\n\
- Parameter par_f_angle is a multiple of 360.0 (including 0.0).",
     "N/A"},

    /* 91: SGL_ERROR_SGLU_ARCELLIPSE2F */
    {
     "SGL_ERROR_SGLU_ARCELLIPSE2F",
     "One of following:\n\
- Parameter par_b_clockwise is different from SGL_TRUE, SGL_FALSE.\n\
- Parameter par_b_as_value is different from SGL_ELLIPSE_FILLED, SGL_ELLIPSE_OUTLINE.",
     "N/A"},

    /* 92: SGL_ERROR_SGL_SETCHARSETENCODING */
    {
     "SGL_ERROR_SGL_SETCHARSETENCODING",
     "Parameter par_b_format is different from SGL_FORMAT_ASCII, SGL_FORMAT_UTF8, SGL_FORMAT_GBK.",
     "par_b_format"},

    /* 93: SGL_ERROR_SGL_PUSHMATRIXANDTRANSLATE */
    {
     "SGL_ERROR_SGL_PUSHMATRIXANDTRANSLATE",
     "The matrix stack used to store the current modelview matrix is full.",
     "N/A"},

    /* 94: SGL_ERROR_SGL_POPMATRIXANDTRANSLATE */
    {
     "SGL_ERROR_SGL_POPMATRIXANDTRANSLATE",
     "The matrix stack used to store the current modelview matrix is empty.",
     "N/A"},

    /* 95: SGL_ERROR_SGL_INDEXFONTLINEWIDTHI */
    {
     "SGL_ERROR_SGL_INDEXFONTLINEWIDTHI",
     "Parameter par_l_index is out of range [0,7].",
     "par_l_index"},

    /* 96: SGL_ERROR_SGL_LINEWIDTHPOINTERF */
    {
     "SGL_ERROR_SGL_LINEWIDTHPOINTERF",
     "One of following:\n\
- Parameter par_ul_nb_line_width is zero.\n\
- Parameter par_ps_linewidth is SGL_NULL.\n\
- One of the line width entry is out of range [0.0,15.0].",
     "N/A"},

    /* 97: SGL_ERROR_SGLU_WRITEUNICODETEXT */
    {
     "SGL_ERROR_SGLU_WRITEUNICODETEXT",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_STRING_LENGTH].\n\
- Parameter par_pl_string is SGL_NULL (error agument set to 0).\n\
- Parameter par_pl_string contains a negative value before index par_l_string_length (error agument set to 0).",
     "par_l_string_length or 0"},

    /* 98: SGL_ERROR_SGL_CREATESTATICBITMAP */
    {
     "SGL_ERROR_SGL_CREATESTATICBITMAP",
     "One of following:\n\
- The number of created static bitmaps exceeds SGL_MAX_STATIC_TEXTURE.\n\
- Parameter par_l_width is out of range [1, <par_ul_texture_max_width of sglInit>].\n\
- Parameter par_l_height is out of range [1, <par_ul_texture_max_height of sglInit>].\n\
- The texture buffer given to sglInit is SGL_NULL.\n\
- par_p_pixel is SGL_NULL.\n\
- Parameter par_b_compressed is SGL_TRUE and, after decompression, the size of the obtained data is different from par_l_width*par_l_height.",
     "Number of created static bitmaps"},

    /* 99: SGL_ERROR_SGL_DRAWSTATICBITMAP */
    {
     "SGL_ERROR_SGL_DRAWSTATICBITMAP",
     "Parameter par_ul_index is not in range [0, <last created static bitmap index>].",
     "par_ul_index"},

    /* 100: SGL_ERROR_SGLU_WRITEWIDETEXT */
    {
     "SGL_ERROR_SGLU_WRITEWIDETEXT",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_STRING_LENGTH].\n\
- Parameter par_pus_string is SGL_NULL (error argument set to 0).",
     "par_l_string_length or 0"},

    /* 101: SGL_ERROR_SGLU_WRITEWIDETEXTAREA */
    {
     "SGL_ERROR_SGLU_WRITEWIDETEXTAREA",
     "One of following:\n\
- Parameter par_l_string_length is out of range [0,SGLU_MAX_TEXT_AREA_STRING_LENGTH].\n\
- Parameter par_pus_string is SGL_NULL (error agument set to 0).",
     "par_l_string_length or 0"},

    /* 102: SGL_ERROR_SGL_INDEXHALOCOLORI */
    {
     "SGL_ERROR_SGL_INDEXHALOCOLORI",
     "Parameter par_l_index is greater or equal to <number of colors>",
     "par_l_index"},

    /* 103: No error */
    {"", "", ""},

    /* 104: SGL_ERROR_SGL_CONVERTTOINT32 */
    {
     "SGL_ERROR_SGL_CONVERTTOINT32",
     "Parameter value does not fit in range of SGLint32.",
     "N/A"},

    /* 105: SGL_ERROR_SGL_CONVERTTOUINT32 */
    {
     "SGL_ERROR_SGL_CONVERTTOUINT32",
     "Parameter value does not fit in range of SGLuint32.",
     "N/A"},

    /* 106: SGL_ERROR_SGL_CONVERTTOFLOAT */
    {
     "SGL_ERROR_SGL_CONVERTTOFLOAT",
     "Parameter value does not fit in range of SGLfloat.",
     "N/A"},

    /* 107: SGL_ERROR_SGL_CONVERTTOINT8 */
    {
     "SGL_ERROR_SGL_CONVERTTOINT8",
     "Parameter value does not fit in range of SGLint8.",
     "N/A"},

    /* 108: SGL_ERROR_SGL_CONVERTTOUINT8 */
    {
     "SGL_ERROR_SGL_CONVERTTOUINT8",
     "Parameter value does not fit in range of SGLuint8.",
     "N/A"},

    /* 109: SGL_ERROR_SGL_CONVERTTOINT16 */
    {
     "SGL_ERROR_SGL_CONVERTTOINT16",
     "Parameter value does not fit in range of SGLint16.",
     "N/A"},

    /* 110: SGL_ERROR_SGL_CONVERTTOUINT16 */
    {
     "SGL_ERROR_SGL_CONVERTTOUINT16",
     "Parameter value does not fit in range of SGLuint16.",
     "N/A"},

    /* 111: SGL_ERROR_SGL_CONVERTTOUINT64 */
    {
     "SGL_ERROR_SGL_CONVERTTOUINT64",
     "Parameter value does not fit in range of SGLuint64.",
     "N/A"},

    /* 112: SGL_ERROR_SGL_CONVERTTOINT64 */
    {
     "SGL_ERROR_SGL_CONVERTTOINT64",
     "Parameter value does not fit in range of SGLint64.",
     "N/A"},

    /* 113: SGL_ERROR_SGL_BEGINPATH */
    {
     "SGL_ERROR_SGL_BEGINPATH",
     "Parameter par_b_mode of sglBeginPath is different from SGL_OUTLINE, SGL_FILL.",
     "par_b_mode"},

    /* 114: SGL_ERROR_SGL_GRADIENTTEXTURE */
    {
     "SGL_ERROR_SGL_GRADIENTTEXTURE",
     "One of following:\n\
- Parameter par_l_index is out of range [0, <ul_number_of_gradients of sglInit>-1].\n\
- Parameter par_l_width is out of range [1, <par_ul_texture_max_width of sglInit>].\n\
- Parameter par_l_height is out of range [1, <par_ul_texture_max_height of sglInit>].\n\
- The texture buffer given to sglInit is SGL_NULL.\n\
- par_p_pixel is SGL_NULL.\n\
- Parameter par_b_compressed is SGL_TRUE and, after decompression, the size of the obtained data is different from par_l_width*par_l_height.",
     "Gradient index"},

    /* 115: SGL_ERROR_SGL_INDEXGRADIENTI */
    {
     "SGL_ERROR_SGL_INDEXGRADIENTI",
     "One of following:\n\
- Parameter par_l_gradient_id is out of range [0,<ul_number_of_gradients of sglInit>-1].\n\
- Parameter par_l_gradient_id specifies a gradient that is not defined.\n\
- The attribute p_gradient_attrib given to sglInit is SGL_NULL.",
     "par_l_gradient_id"},

    /* 116: No Error */
    {
     "SGL_ERROR_SGL_BEGINMASKDEFINITION",
     "Parameter par_b_visible is different from SGL_VISIBLE_INSIDE, SGL_VISIBLE_OUTSIDE.",
     "N/A"},

    /* 117: SGL_ERROR_SGL_FORMATGRADIENT */
    {
     "SGL_ERROR_SGL_FORMATGRADIENT",
     "One of following:\n\
- Parameter par_ul_nb_vertices is out of range [2,SGL_MAX_VERTEX_ARRAY_SIZE-1].\n\
- Parameter par_pf_x is SGL_NULL.\n\
- Parameter par_pf_y is SGL_NULL.\n\
- The attribute p_gradient_attrib given to sglInit is SGL_NULL.",
     "par_ul_nb_vertices"},

    /* 118: SGL_ERROR_SGL_CALLSTATICSEQUENCE */
    {
     "SGL_ERROR_SGL_CALLSTATICSEQUENCE",
     "One of following:\n\
- Parameter par_ul_index is out of range [0, SGL_MAX_STATIC_TEXTURE-1] and driver OpenGL ES 2.0 or OpenGL SC 2.0 is used.\n\
- Parameter par_ul_index is not the index of a defined static sequence and driver OpenGL ES 2.0 or OpenGL SC 2.0 is used.",
     "par_ul_index"},

    /* 119: SGL_ERROR_SGLU_DOUBLEDIV */
    {
     "SGL_ERROR_SGLU_DOUBLEDIV",
     "Parameter par_d_double_2 value is 0.0 with a precision of SGL_MIN_D.",
     "N/A"},

    /* 120: SGL_ERROR_SGL_CONVERTPOINTTOPIXEL */
    {
     "SGL_ERROR_SGL_CONVERTPOINTTOPIXEL",
     "par_pf_pixel_x or par_pf_pixel_y is SGL_NULL",
     "N/A"},

    /* 121: SGL_ERROR_SGL_CONVERTPOINTTOSCREEN */
    {
     "SGL_ERROR_SGL_CONVERTPOINTTOSCREEN",
     "par_pf_screen_x or par_pf_screen_y is SGL_NULL",
     "N/A"},

    /* 122: SGL_ERROR_SGL_CONVERTPIXELTOPOINT */
    {
     "SGL_ERROR_SGL_CONVERTPIXELTOPOINT",
     "par_pf_user_x or par_pf_user_y is SGL_NULL",
     "N/A"},

    /* 123: SGL_ERROR_SGL_CONVERTSCREENTOPOINT */
    {
     "SGL_ERROR_SGL_CONVERTSCREENTOPOINT",
     "par_pf_user_x or par_pf_user_y is SGL_NULL",
     "N/A"},

    /* 124: SGL_ERROR_SGL_CUBICBEZIERBOUNDS */
    {
     "SGL_ERROR_SGL_CUBICBEZIERBOUNDS",
     "par_pf_x1, par_pf_y1, par_pf_x2 or par_pf_y2 is SGL_NULL",
     "N/A"},

    /* 125: SGL_ERROR_SGL_SMOOTHCUBICBEZIERBOUNDS */
    {
     "SGL_ERROR_SGL_SMOOTHCUBICBEZIERBOUNDS",
     "par_pf_x1, par_pf_y1, par_pf_x2 or par_pf_y2 is SGL_NULL",
     "N/A"},

    /* 126: SGL_ERROR_SGL_ELLIPTICALARCBOUNDS */
    {
     "SGL_ERROR_SGL_ELLIPTICALARCBOUNDS",
     "par_pf_x1, par_pf_y1, par_pf_x2 or par_pf_y2 is SGL_NULL",
     "N/A"},

    /* 127: SGL_ERROR_SGL_QUADBEZIERBOUNDS */
    {
     "SGL_ERROR_SGL_QUADBEZIERBOUNDS",
     "par_pf_x1, par_pf_y1, par_pf_x2 or par_pf_y2 is SGL_NULL",
     "N/A"},
    /* 128: SGL_ERROR_SGL_SMOOTHQUADBEZIERBOUNDS */
    {
     "SGL_ERROR_SGL_SMOOTHQUADBEZIERBOUNDS",
     "par_pf_x1, par_pf_y1, par_pf_x2 or par_pf_y2 is SGL_NULL",
     "N/A"},
};


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxReadErrors
  DESCRIPTION:
    Function shall shall provide a readable strucure for errors stored.
  PARAMETERS:
    None
  RETURN:
    The bounding box of the objects drawn since sglBeginBounds
---------------------------------------------------------------------+*/
oglx_error *oglxReadErrors(SGLulong * par_ul_error_number)
{
    SGLulong loc_ul_index;
    SGLulong loc_ul_error_number = glob_pr_sglStatemachine->ul_error_counter;
    *par_ul_error_number = loc_ul_error_number;

    for (loc_ul_index = 0UL; loc_ul_index < loc_ul_error_number; loc_ul_index++) {
        SGLulong loc_ul_error_id = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][0];
        SGLulong loc_ul_error_parameter = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][1];
        if (loc_ul_error_id < OGL_ERRORS_NUMBER) {
            oglx_error_definition loc_error = oglxGetErrorDefinition(loc_ul_error_id);
            glob_s_error_table[loc_ul_index].s_error_name = loc_error.s_error_name;
            glob_s_error_table[loc_ul_index].s_rationale = loc_error.s_rationale;
            glob_s_error_table[loc_ul_index].s_parameter_name = loc_error.s_parameter_name;
            glob_s_error_table[loc_ul_index].ul_parameter_value = loc_ul_error_parameter;
        }
        else {
            /* Nothing to do */
        }
    }

    return glob_s_error_table;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxGetErrorDefinition
  DESCRIPTION:
    Function shall shall provide a readable strucure for the required error.
  PARAMETERS:
    par_ul_error_index -> index of the error
  RETURN:
    A structure containing as text the error name, the error rationale and the parameter of the error
---------------------------------------------------------------------+*/
oglx_error_definition oglxGetErrorDefinition(SGLulong par_ul_error_index)
{
    return glob_errors_definition[par_ul_error_index];
}


/* End of File ***************************************************************/
