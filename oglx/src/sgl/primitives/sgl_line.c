/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_line.c
 DESCRIPTION       : Set of functions used to draw lines
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

/******************************************************************************
 **                           Local file declarations
 *****************************************************************************/

#define SGL_EPS			((SGLfloat)2.44140625E-4F)

#define SGL_2_5_DIV_1_75 ((SGLfloat)1.4285715F)

/* Set alpha offest according to the haloing */
#define SGL_ALPHA_OFFSET ((SGLfloat)10.0F)
#define SGL_INV_AOFFSET_COS ((SGLfloat) 1.0154266F)
#define SGL_AOFFSET_SIN ((SGLfloat) 0.1736482F)

sgl_line_width font_line_width[FONT_LINE_WIDTH_SIZE] = {
    {1.5F, 1.5F, 3.5F}
    ,                           /* LW = 1 */
    {2.4F, 1.5F, 3.5F}
    ,                           /* LW = 2 */
    {2.55F, 1.65F, 3.65F}
    ,                           /* LW = 3 */
    {2.75F, 2.25F, 4.25F}
    ,                           /* LW = 4 */
    {3.55F, 2.55F, 4.55F}
    ,                           /* LW = 5 */
    {4.55F, 3.55F, 5.55F}
    ,                           /* LW = 6 */
    {5.55F, 4.25F, 6.25F}
    ,                           /* LW = 7 */
    {6.55F, 5.00F, 7.00F}
    ,                           /* LW = 8 */
};

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_set_line_width
  DESCRIPTION:
    Function shall change current OpenGL line width, if it is different from the active one.
  SCOPE: global
  PARAMETERS:
    par_f_line_width -> the line width to apply			
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_line_width(SGLfloat par_f_line_width)
{
    if (!sgluFloatIsEqual(par_f_line_width, glob_pr_sglStatemachine->f_last_ogl_linewidth)) {
        SGLfloat loc_f_factor = (glob_pr_sglStatemachine->f_width_factor + glob_pr_sglStatemachine->f_height_factor) * 0.5F;
        glob_pr_sglStatemachine->f_last_ogl_linewidth = par_f_line_width;
        glLineWidth(par_f_line_width * loc_f_factor);
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_arrays_gl_lines
  DESCRIPTION:
    Function shall directly draw current vertex array with GL_LINES mode.
  SCOPE: global
  PARAMETERS:
    None			
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_arrays_gl_lines(void)
{
    void *loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_array_vertices;
#ifndef ES2_DEV_ENV
    oglxDrawVertexArray(GL_LINES, glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices, (const GLfloat *) loc_p_vertex_array);
#else
    oglxDrawVertexArray_es2(GL_LINES, glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices, (const GLfloat *) loc_p_vertex_array);
#endif
    glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices = 0;
}




/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_simple_line
  DESCRIPTION:
    Function shall define vertices of simple lines
  SCOPE: global
  PARAMETERS:
    par_f_x1 -> x-parameter of the start point
    par_f_y1 -> y-parameter of the start point
    par_f_x2 -> x-parameter of the end point
    par_f_y2 -> y-parameter of the end point
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_simple_line(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLlong loc_l_size;

    /* Avoid vertex array overflow by flushing it when it becomes full */
    if ((glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices + 2) > (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE) {
        sgl_draw_arrays_gl_lines();
    }
    else {
        /* Nothing to do */
    }

    loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices;

    glob_pr_sglStatemachine->tr_geometry.p_array_vertices[loc_l_size][0] = par_f_x1;
    glob_pr_sglStatemachine->tr_geometry.p_array_vertices[loc_l_size][1] = par_f_y1;

    glob_pr_sglStatemachine->tr_geometry.p_array_vertices[loc_l_size + 1][0] = par_f_x2;
    glob_pr_sglStatemachine->tr_geometry.p_array_vertices[loc_l_size + 1][1] = par_f_y2;

    glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices += 2;

    return;
}




/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_raw_lines
  DESCRIPTION:
    Function shall draw raw OpenGL lines.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_raw_lines(void)
{
    SGLlong loc_l_i;
    SGLlong loc_l_inc;

    loc_l_inc = 2L;

    for (loc_l_i = 0; loc_l_i < (glob_pr_sglStatemachine->tr_geometry.l_size - 1); loc_l_i += loc_l_inc) {
        sgl_draw_simple_line(glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i][0], glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i][1],
                             glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i + 1][0], glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i + 1][1]);
    }

    sgl_draw_arrays_gl_lines();

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_raw_lines_loop_strip
  DESCRIPTION:
    Function shall draw raw OpenGL lines loop/strip
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_raw_lines_loop_strip(void)
{
    void *loc_p_vertex_array;

    if (glob_pr_sglStatemachine->tr_geometry.b_type == SGL_LINE_LOOP) {
        loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#ifndef ES2_DEV_ENV
        oglxDrawVertexArray(GL_LINE_LOOP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#else
        oglxDrawVertexArray_es2(GL_LINE_LOOP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#endif
    }
    else {
        loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#ifndef ES2_DEV_ENV
        oglxDrawVertexArray(GL_LINE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#else
        oglxDrawVertexArray_es2(GL_LINE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#endif
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_smooth_single_line
  DESCRIPTION:
    Function shall define vertex of antialiazed line with round ends
  SCOPE: global
  PARAMETERS:
    par_pr_smooth_line -> pointer to structure containing parameters:
      us_nb_lines -> number of lines to draw
      b_start -> SGL_TRUE if first point is the start point of the line, SGL_FALSE otherwise
      b_end -> SGL_TRUE if second point is the end point of the line, SGL_FALSE otherwise
      f_x1 -> x-parameter of the start point
      f_y1 -> y-parameter of the start point
      f_x2 -> x-parameter of the end point
      f_y2 -> y-parameter of the end point
      f_sa_ratio -> sine for shift of center extremity for beam
      f_ray -> ray for beam of lines
      f_ca_tangent -> for straight caps, cosine of tangent
      f_sa_tangent -> for straight caps, sine of tangent
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_smooth_single_line(sgl_smooth_line * par_pr_smooth_line)
{
    SGLfloat loc_f_dx_norm;
    SGLfloat loc_f_dy_norm;
    SGLfloat loc_f_center_decal_x;
    SGLfloat loc_f_center_decal_y;

    SGLfloat loc_f_dx = par_pr_smooth_line->f_x2 - par_pr_smooth_line->f_x1;
    SGLfloat loc_f_dy = par_pr_smooth_line->f_y2 - par_pr_smooth_line->f_y1;

    /* 
       When two extrem vertices of a line have the same coordinates, OGLX does not draw anything.
       To prevent that, check the absolute value of length and compare it with the pitch value
     */
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 10, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    if ((loc_f_dx > glob_pr_sglStatemachine->f_ratio_x) || (-loc_f_dx > glob_pr_sglStatemachine->f_ratio_x) ||
        (loc_f_dy > glob_pr_sglStatemachine->f_ratio_y) || (-loc_f_dy > glob_pr_sglStatemachine->f_ratio_y)) {
        /* At least one pixel will be drawn. No correction necessary */
    }
    else {
        if (mth_f_abs(loc_f_dx) > SGL_EPS) {
            /* Modify one of the vertices so that they are different.
               Due to antialiasing being performed horizontally, shift x coordinate. */
            if (loc_f_dx > SGL_EPS) {
                par_pr_smooth_line->f_x2 = par_pr_smooth_line->f_x1 + glob_pr_sglStatemachine->f_ratio_x;
            }
            else {
                par_pr_smooth_line->f_x2 = par_pr_smooth_line->f_x1 - glob_pr_sglStatemachine->f_ratio_x;
            }
        }
        else {
            /* Same x coordinate move coordinate y */
            if (loc_f_dy > SGL_EPS) {
                par_pr_smooth_line->f_y2 = par_pr_smooth_line->f_y1 + glob_pr_sglStatemachine->f_ratio_y;
            }
            else {
                par_pr_smooth_line->f_y2 = par_pr_smooth_line->f_y1 - glob_pr_sglStatemachine->f_ratio_y;
            }
        }
    }

    /* Compute norm */
    {
        SGLfloat loc_f_norm = ((loc_f_dx * loc_f_dx) + (loc_f_dy * loc_f_dy));
        loc_f_norm = mth_sqrtf(loc_f_norm);

        if (loc_f_norm > SGL_MIN_F) {
            loc_f_dx_norm = SGLfloat_div(loc_f_dx, loc_f_norm);
            loc_f_dy_norm = SGLfloat_div(loc_f_dy, loc_f_norm);
        }
        else {
            loc_f_dx_norm = 1.0F;
            loc_f_dy_norm = 0.0F;
        }
    }

    /* Compute center of extremities of lines */
    loc_f_center_decal_x = loc_f_dx_norm * par_pr_smooth_line->f_sa_ratio;
    loc_f_center_decal_y = loc_f_dy_norm * par_pr_smooth_line->f_sa_ratio;

    /* if two lines at least have to be drawn, the round shape is drawn */
    if (par_pr_smooth_line->us_nb_lines > 1U) {
        SGLushort loc_us_k;
        SGLfloat loc_f_ray_dx;
        SGLfloat loc_f_ray_dy;
        SGLfloat loc_f_sa;
        SGLfloat loc_f_ca;
        SGLfloat loc_f_sa_dx;
        SGLfloat loc_f_sa_dy;
        SGLfloat loc_f_ca_dx;
        SGLfloat loc_f_ca_dy;

        SGLfloat loc_f_center_x1 = par_pr_smooth_line->f_x1 + loc_f_center_decal_x;
        SGLfloat loc_f_center_y1 = par_pr_smooth_line->f_y1 + loc_f_center_decal_y;
        SGLfloat loc_f_center_x2 = par_pr_smooth_line->f_x2 - loc_f_center_decal_x;
        SGLfloat loc_f_center_y2 = par_pr_smooth_line->f_y2 - loc_f_center_decal_y;

        /* size of circle ray */
        loc_f_ray_dx = par_pr_smooth_line->f_ray * loc_f_dx_norm;
        loc_f_ray_dy = par_pr_smooth_line->f_ray * loc_f_dy_norm;

        if ((glob_pr_sglStatemachine->l_line_caps_appearance == SGL_ROUNDED_CAPS) || ((!par_pr_smooth_line->b_start) && (!par_pr_smooth_line->b_end))) {
            for (loc_us_k = 1U; loc_us_k <= par_pr_smooth_line->us_nb_lines; loc_us_k++) {
                /* loc_us_nb_lines and loc_us_k are both less than OGLX_MAX_LINES. Their maximum value is 5 */
                /* OGLX_MAX_LINES value is 16 as it is a provision for future versions with potential larger lines */
                loc_f_sa = glob_pr_sglStatemachine->pf_sin[par_pr_smooth_line->us_nb_lines][loc_us_k];
                loc_f_ca = glob_pr_sglStatemachine->pf_cos[par_pr_smooth_line->us_nb_lines][loc_us_k];
                loc_f_sa_dx = loc_f_sa * loc_f_ray_dx;
                loc_f_sa_dy = loc_f_sa * loc_f_ray_dy;
                loc_f_ca_dx = loc_f_ca * loc_f_ray_dx;
                loc_f_ca_dy = loc_f_ca * loc_f_ray_dy;

                sgl_draw_simple_line((loc_f_center_x1 - (loc_f_ca_dy + loc_f_sa_dx)),
                                     (loc_f_center_y1 + (loc_f_ca_dx - loc_f_sa_dy)),
                                     (loc_f_center_x2 + (loc_f_sa_dx - loc_f_ca_dy)), (loc_f_center_y2 + (loc_f_ca_dx + loc_f_sa_dy)));
            }
        }
        else {
            SGLfloat loc_f_prev_x1;
            SGLfloat loc_f_prev_y1;
            SGLfloat loc_f_prev_x2;
            SGLfloat loc_f_prev_y2;

            SGLfloat loc_f_x1;
            SGLfloat loc_f_y1;
            SGLfloat loc_f_x2;
            SGLfloat loc_f_y2;
            SGLfloat loc_f_u;
            SGLfloat loc_f_v;
            SGLfloat loc_f_a1 = 0.0F;
            SGLfloat loc_f_b1 = 0.0F;
            SGLfloat loc_f_a3 = 0.0F;
            SGLfloat loc_f_b3 = 0.0F;
            SGLfloat loc_f_a2;
            SGLfloat loc_f_b2;

            /* Compute tangent coefficients */
            loc_f_sa_dx = par_pr_smooth_line->f_sa_tangent * loc_f_ray_dx;
            loc_f_sa_dy = par_pr_smooth_line->f_sa_tangent * loc_f_ray_dy;
            loc_f_ca_dx = par_pr_smooth_line->f_ca_tangent * loc_f_ray_dx;
            loc_f_ca_dy = par_pr_smooth_line->f_ca_tangent * loc_f_ray_dy;

            loc_f_x1 = (loc_f_center_x1 - (loc_f_ca_dy + loc_f_sa_dx));
            loc_f_y1 = (loc_f_center_y1 + (loc_f_ca_dx - loc_f_sa_dy));
            loc_f_x2 = (loc_f_center_x2 + (loc_f_sa_dx - loc_f_ca_dy));
            loc_f_y2 = (loc_f_center_y2 + (loc_f_ca_dx + loc_f_sa_dy));

            loc_f_u = loc_f_x2 - loc_f_x1;
            loc_f_v = loc_f_y2 - loc_f_y1;

            loc_f_prev_x1 = loc_f_x1;
            loc_f_prev_y1 = loc_f_y1;

            loc_f_prev_x2 = loc_f_x2;
            loc_f_prev_y2 = loc_f_y2;

            if (!sgluFloatIsEqual(loc_f_v, 0.0F)) {
                if (!sgluFloatIsEqual(loc_f_u, 0.0F)) {
                    loc_f_a1 = SGLfloat_div(-loc_f_u, loc_f_v);
                    loc_f_b1 = SGLfloat_div((loc_f_x1 * loc_f_u), loc_f_v) + loc_f_y1;
                    loc_f_a3 = SGLfloat_div(-loc_f_u, loc_f_v);
                    loc_f_b3 = SGLfloat_div((loc_f_x2 * loc_f_u), loc_f_v) + loc_f_y2;
                }
                else {
                    loc_f_a1 = 1.0F;
                    loc_f_b1 = 0.0F;
                    loc_f_a3 = 1.0F;
                    loc_f_b3 = 0.0F;
                }
            }
            else {
                loc_f_a1 = 0.0F;
                loc_f_b1 = loc_f_y1;
                loc_f_a3 = 0.0F;
                loc_f_b3 = loc_f_y2;
            }

            for (loc_us_k = 1U; loc_us_k <= par_pr_smooth_line->us_nb_lines; loc_us_k++) {
                /* loc_us_nb_lines and loc_us_k are both less than OGLX_MAX_LINES. Their maximum value is 5 */
                /* OGLX_MAX_LINES value is 16 as it is a provision for future versions with potential larger lines */
                loc_f_sa = glob_pr_sglStatemachine->pf_sin[par_pr_smooth_line->us_nb_lines][loc_us_k];
                loc_f_ca = glob_pr_sglStatemachine->pf_cos[par_pr_smooth_line->us_nb_lines][loc_us_k];
                loc_f_sa_dx = loc_f_sa * loc_f_ray_dx;
                loc_f_sa_dy = loc_f_sa * loc_f_ray_dy;
                loc_f_ca_dx = loc_f_ca * loc_f_ray_dx;
                loc_f_ca_dy = loc_f_ca * loc_f_ray_dy;

                loc_f_x1 = (loc_f_center_x1 - (loc_f_ca_dy + loc_f_sa_dx));
                loc_f_y1 = (loc_f_center_y1 + (loc_f_ca_dx - loc_f_sa_dy));
                loc_f_x2 = (loc_f_center_x2 + (loc_f_sa_dx - loc_f_ca_dy));
                loc_f_y2 = (loc_f_center_y2 + (loc_f_ca_dx + loc_f_sa_dy));
                loc_f_u = loc_f_x2 - loc_f_x1;
                loc_f_v = loc_f_y2 - loc_f_y1;

                if (!sgluFloatIsEqual(loc_f_u, 0.0F)) {
                    if (!sgluFloatIsEqual(loc_f_v, 0.0F)) {
                        loc_f_a2 = SGLfloat_div(loc_f_v, loc_f_u);
                        loc_f_b2 = loc_f_y1 - SGLfloat_div((loc_f_v * loc_f_x1), loc_f_u);
                        if (par_pr_smooth_line->b_start) {
                            loc_f_x1 = SGLfloat_div(loc_f_b2 - loc_f_b1, loc_f_a1 - loc_f_a2);
                            loc_f_y1 = SGLfloat_div((loc_f_a1 * loc_f_b2) - (loc_f_a2 * loc_f_b1)
                                                    , loc_f_a1 - loc_f_a2);
                        }
                        if (par_pr_smooth_line->b_end) {
                            loc_f_x2 = SGLfloat_div(loc_f_b2 - loc_f_b3, loc_f_a3 - loc_f_a2);
                            loc_f_y2 = SGLfloat_div((loc_f_a3 * loc_f_b2) - (loc_f_a2 * loc_f_b3)
                                                    , loc_f_a3 - loc_f_a2);
                        }
                    }
                    else {
                        if (par_pr_smooth_line->b_start) {
                            loc_f_x1 = loc_f_prev_x1;
                        }
                        if (par_pr_smooth_line->b_end) {
                            loc_f_x2 = loc_f_prev_x2;
                        }
                    }
                }
                else {
                    if (par_pr_smooth_line->b_start) {
                        loc_f_y1 = loc_f_prev_y1;
                    }
                    if (par_pr_smooth_line->b_end) {
                        loc_f_y2 = loc_f_prev_y2;
                    }
                }

                sgl_draw_simple_line(loc_f_x1, loc_f_y1, loc_f_x2, loc_f_y2);
            }
        }
    }
    else {
        sgl_draw_simple_line((par_pr_smooth_line->f_x1 - loc_f_center_decal_x), (par_pr_smooth_line->f_y1 - loc_f_center_decal_y),
                             (par_pr_smooth_line->f_x2 + loc_f_center_decal_x), (par_pr_smooth_line->f_y2 + loc_f_center_decal_y));
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_smooth_lines
  DESCRIPTION:
    Function shall define smooth lines
  SCOPE: global
  PARAMETERS:
    par_f_half_pixel_width -> width for a half pixel
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_smooth_lines(SGLfloat par_f_half_pixel_width)
{
    SGLlong loc_l_i;
    SGLlong loc_l_inc;
    SGLlong loc_l_size_max;
    SGLlong loc_l_size_max_minus_one;
    SGLbool loc_b_not_line_loop;
    SGLbool loc_b_lines;

    sgl_smooth_line loc_tr_smooth_line;

    SGLfloat loc_l_ratio = (glob_pr_sglStatemachine->f_ratio_x + glob_pr_sglStatemachine->f_ratio_y) * 0.5F;

    loc_tr_smooth_line.f_sa_ratio = loc_l_ratio * SGL_AOFFSET_SIN;

    if (glob_pr_sglStatemachine->tr_geometry.b_type == SGL_LINES) {
        loc_l_inc = 2L;
        loc_b_lines = SGL_TRUE;
    }
    else {
        loc_l_inc = 1L;
        loc_b_lines = SGL_FALSE;
    }

    if (glob_pr_sglStatemachine->tr_geometry.b_type == SGL_LINE_LOOP) {
        loc_b_not_line_loop = SGL_FALSE;
    }
    else {
        loc_b_not_line_loop = SGL_TRUE;
    }

    /* Simple draw for small line width */
    if (par_f_half_pixel_width < 1.0F) {
        loc_tr_smooth_line.us_nb_lines = 1U;
        loc_tr_smooth_line.f_ray = 0.0F;
        loc_tr_smooth_line.f_sa_tangent = 0.0F;
        loc_tr_smooth_line.f_ca_tangent = 1.0F;
    }
    else {
        SGLfloat loc_f_nb_lines = SGL_2_5_DIV_1_75 * par_f_half_pixel_width;
        SGLushort loc_us_nb_lines = (SGLushort) loc_f_nb_lines;
        if (loc_us_nb_lines > (SGLushort) par_f_half_pixel_width) {
            loc_us_nb_lines++;
        }
        else {
            /* Nothing to do */
        }

        if ((loc_us_nb_lines & 1U) == 0U) {
            /* Prefer an odd number of drawings to draw a rounder shape */
            loc_us_nb_lines++;
        }
        else {
            /* Nothing to do */
        }

        /* Compute ray */
        loc_tr_smooth_line.us_nb_lines = loc_us_nb_lines;
        loc_tr_smooth_line.f_ray = ((loc_l_ratio * (par_f_half_pixel_width - 0.875F)) * SGL_INV_AOFFSET_COS);

        /* Compute tangent if straigth caps */
        if (glob_pr_sglStatemachine->l_line_caps_appearance == SGL_ROUNDED_CAPS) {
            loc_tr_smooth_line.f_sa_tangent = 0.0F;
            loc_tr_smooth_line.f_ca_tangent = 1.0F;
        }
        else {
            SGLfloat loc_f_tangent_angle = SGLfloat_div(180.0F - (2.0F * SGL_ALPHA_OFFSET), (SGLfloat) loc_us_nb_lines + 1.0F);
            loc_f_tangent_angle = SGL_ALPHA_OFFSET + (loc_f_tangent_angle * (((SGLfloat) ((SGLlong) loc_us_nb_lines)) * 0.5F));
            loc_tr_smooth_line.f_sa_tangent = mth_sin_degree(loc_f_tangent_angle);
            loc_tr_smooth_line.f_ca_tangent = mth_cos_degree(loc_f_tangent_angle);
        }
    }


    /* Call the draw of each segment of the line */
    loc_l_size_max = glob_pr_sglStatemachine->tr_geometry.l_size - 1;
    loc_l_size_max_minus_one = loc_l_size_max - 1;
    for (loc_l_i = 0; loc_l_i < loc_l_size_max; loc_l_i += loc_l_inc) {
        if ((loc_b_not_line_loop && (loc_l_i == 0)) || loc_b_lines) {
            loc_tr_smooth_line.b_start = SGL_TRUE;
        }
        else {
            loc_tr_smooth_line.b_start = SGL_FALSE;
        }

        if ((loc_b_not_line_loop && (loc_l_i == loc_l_size_max_minus_one)) || loc_b_lines) {
            loc_tr_smooth_line.b_end = SGL_TRUE;
        }
        else {
            loc_tr_smooth_line.b_end = SGL_FALSE;
        }

        loc_tr_smooth_line.f_x1 = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i][0];
        loc_tr_smooth_line.f_y1 = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i][1];
        loc_tr_smooth_line.f_x2 = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i + 1][0];
        loc_tr_smooth_line.f_y2 = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_i + 1][1];

        sgl_draw_smooth_single_line(&loc_tr_smooth_line);
    }

    /* Call the draw of the last segment of the line if it is a line loop */
    if (!loc_b_not_line_loop) {
        loc_tr_smooth_line.b_start = SGL_FALSE;
        loc_tr_smooth_line.b_end = SGL_FALSE;
        loc_tr_smooth_line.f_x1 = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size_max][0];
        loc_tr_smooth_line.f_y1 = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size_max][1];
        loc_tr_smooth_line.f_x2 = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0];
        loc_tr_smooth_line.f_y2 = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1];

        sgl_draw_smooth_single_line(&loc_tr_smooth_line);
    }
    else {
        /* do nothing */
    }

    sgl_draw_arrays_gl_lines();

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxTranslateZ
  DESCRIPTION:
    Function shall translate the line in Z
  SCOPE: global
  PARAMETERS:
    par_f_z -> translate value
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxTranslateZ(SGLfloat par_f_z)
{
    glob_pr_sglStatemachine->f_modelview_matrix[14] = par_f_z;

#ifndef ES2_DEV_ENV
    glTranslatef(0.0F, 0.0F, glob_pr_sglStatemachine->f_modelview_matrix[14] - glob_pr_sglStatemachine->f_current_depth);
#else
    oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif
    glob_pr_sglStatemachine->f_current_depth = glob_pr_sglStatemachine->f_modelview_matrix[14];
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_continuous_lines
  DESCRIPTION:
    Function shall initiate lines drawing
  SCOPE: global
  PARAMETERS:
    par_f_alpha_halo -> width of haloing
    par_f_half_pixel_width_haloed -> half of the width of haloing
    par_f_half_pixel_width -> half of pixel width
    par_b_haloed_part -> SGL_TRUE if there is halo, else SGL_FALSE
    par_b_colored_part ->SGL_TRUE if there is color instead of halo, else SGL_FALSE
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_continuous_lines(SGLfloat par_f_alpha_halo, SGLfloat par_f_half_pixel_width_haloed, SGLfloat par_f_half_pixel_width,
                               SGLbyte par_b_haloed_part, SGLbyte par_b_colored_part)
{
    SGLlong loc_l_index_line_width = glob_pr_sglStatemachine->l_linewidth_index;
    SGLfloat loc_f_line_width = 2.0F;

    if (glob_pr_sglStatemachine->tr_geometry.b_type == SGL_LINES) {
        if (glob_pr_sglStatemachine->b_line_rendering_mode == SGL_RAW_OPENGL_LINES) {
            SGLfloat loc_f_depth = glob_pr_sglStatemachine->f_current_depth;
            /* Draw haloed part for raw lines */
            if (par_b_haloed_part) {
                SGLfloat loc_f_halo_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_halo_width;
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth - SGL_HALO_TRANSLATE);
                }

                if (glob_pr_sglStatemachine->l_halo_color_index < 0L) {
                    oglxColor4f(0.0F, 0.0F, 0.0F, par_f_alpha_halo);
                }
                else {
                    oglxUpdateColor(glob_pr_sglStatemachine->l_halo_color_index);
                }

                sgl_set_line_width(loc_f_halo_width);
                sgl_draw_raw_lines();
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
            }

            if (glob_pr_sglStatemachine->b_haloing_state) {
                loc_f_line_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_color_width;
            }
            else {
                loc_f_line_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_line_width;
            }

            /* Draw colored part for raw lines */
            if (par_b_colored_part) {
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    oglxTranslateZ(loc_f_depth - 2.0F * SGL_HALO_TRANSLATE);
                }
                oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);
                sgl_set_line_width(loc_f_line_width);
                sgl_draw_raw_lines();
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    /* Restore previous depth */
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
                /* do nothing */
            }
        }
        else {
            SGLfloat loc_f_depth = glob_pr_sglStatemachine->f_current_depth;

            /* Draw haloed part for smooth lines */
            if (par_b_haloed_part) {
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth - SGL_HALO_TRANSLATE);
                }

                if (glob_pr_sglStatemachine->l_halo_color_index < 0L) {
                    oglxColor4f(0.0F, 0.0F, 0.0F, par_f_alpha_halo);
                }
                else {
                    oglxUpdateColor(glob_pr_sglStatemachine->l_halo_color_index);
                }

                sgl_draw_smooth_lines(par_f_half_pixel_width_haloed);
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
            }
            /* Draw colored part for smooth lines */
            if (par_b_colored_part) {
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    oglxTranslateZ(loc_f_depth - 2.0F * SGL_HALO_TRANSLATE);
                }
                oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);

                oglxEnable(GL_BLEND);
                sgl_draw_smooth_lines(par_f_half_pixel_width);
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    /* Restore previous depth */
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
                /* do nothing */
            }
        }
    }
    else {
        if (glob_pr_sglStatemachine->b_line_rendering_mode == SGL_RAW_OPENGL_LINES) {
            SGLfloat loc_f_depth = glob_pr_sglStatemachine->f_current_depth;
            /* Draw haloed part for raw line loop/strip */
            if (par_b_haloed_part) {
                SGLfloat loc_f_halo_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_halo_width;
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth - SGL_HALO_TRANSLATE);
                }

                if (glob_pr_sglStatemachine->l_halo_color_index < 0L) {
                    oglxColor4f(0.0F, 0.0F, 0.0F, par_f_alpha_halo);
                }
                else {
                    oglxUpdateColor(glob_pr_sglStatemachine->l_halo_color_index);
                }

                sgl_set_line_width(loc_f_halo_width);
                sgl_draw_raw_lines_loop_strip();

                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
            }

            if (glob_pr_sglStatemachine->b_haloing_state) {
                loc_f_line_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_color_width;
            }
            else {
                loc_f_line_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_line_width;
            }

            /* Draw colored part for raw line loop/strip */
            if (par_b_colored_part) {
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    oglxTranslateZ(loc_f_depth - 2.0F * SGL_HALO_TRANSLATE);
                }
                /* Restore the color */
                oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);
                sgl_set_line_width(loc_f_line_width);
                sgl_draw_raw_lines_loop_strip();
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            SGLfloat loc_f_depth = glob_pr_sglStatemachine->f_current_depth;

            /* Draw haloed part for smooth line loop/strip */
            if (par_b_haloed_part) {
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth - SGL_HALO_TRANSLATE);
                }
                if (glob_pr_sglStatemachine->l_halo_color_index < 0L) {
                    oglxColor4f(0.0F, 0.0F, 0.0F, par_f_alpha_halo);
                }
                else {
                    oglxUpdateColor(glob_pr_sglStatemachine->l_halo_color_index);
                }

                sgl_draw_smooth_lines(par_f_half_pixel_width_haloed);

                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxTranslateZ(loc_f_depth);
                }
            }
            else {
            }

            /* Draw colored part for smooth line loop/strip */
            if (par_b_colored_part) {

                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    oglxTranslateZ(loc_f_depth - 2.0F * SGL_HALO_TRANSLATE);
                }

                oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);

                oglxEnable(GL_BLEND);
                sgl_draw_smooth_lines(par_f_half_pixel_width);
                if (glob_pr_sglStatemachine->b_halo_level_state && glob_pr_sglStatemachine->b_haloing_state) {
                    oglxTranslateZ(loc_f_depth);
                }

            }
            else {
                /* do nothing */
            }
        }
    }

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_line
  DESCRIPTION:
    Function shall define line drawing algorithms: it is used 
    to draw any geometric shape composed of lines (lines, circles, 
    arcs, linestrips, lineloops) stippled or not.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_line(void)
{
    SGLfloat loc_f_half_pixel_width;
    SGLfloat loc_f_half_pixel_width_haloed;
    SGLfloat loc_f_alpha_halo;
    SGLbyte loc_b_haloed_part;
    SGLbyte loc_b_colored_part;
    SGLlong loc_l_index_line_width = glob_pr_sglStatemachine->l_linewidth_index;

    switch (glob_pr_sglStatemachine->b_haloing_state) {
    case SGL_TRUE:
        loc_b_haloed_part = SGL_TRUE;
        loc_b_colored_part = SGL_TRUE;
        break;
    case SGL_HALOED_PART_ONLY:
        loc_b_haloed_part = SGL_TRUE;
        loc_b_colored_part = SGL_FALSE;
        break;
    default:                   /* SGL_FALSE or SGL_COLORED_PART_ONLY */
        loc_b_haloed_part = SGL_FALSE;
        loc_b_colored_part = SGL_TRUE;
        break;
    }

    /* loc_f_alpha_halo = ((SGLfloat) glob_pr_sglStatemachine->b_transparency) / ((SGLfloat) SGL_MAX_ALPHA_8); */
    loc_f_alpha_halo = ((SGLfloat) glob_pr_sglStatemachine->b_transparency) * SGL_ONE_DIV_MAX_ALPHA_8;

    /* Compute the pixel width for smooth mode */
    if (glob_pr_sglStatemachine->b_line_rendering_mode == SGL_SMOOTH_LINES) {
        SGLfloat loc_f_pixel_width;
        if (!(glob_pr_sglStatemachine->b_haloing_state)) {
            SGLfloat loc_f_line_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_line_width;
            loc_f_half_pixel_width = 0.5F * loc_f_line_width;
            loc_f_half_pixel_width_haloed = 0.0F;
        }
        else {
            SGLfloat loc_f_line_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_color_width;
            SGLfloat loc_f_halo_width = glob_pr_sglStatemachine->tr_line_width[loc_l_index_line_width].f_halo_width;
            loc_f_half_pixel_width = 0.5F * loc_f_line_width;
            loc_f_half_pixel_width_haloed = 0.5F * loc_f_halo_width;
        }

        if (loc_f_half_pixel_width < 1.0F) {
            loc_f_pixel_width = 2.0F * loc_f_half_pixel_width;
        }
        else {
            loc_f_pixel_width = 2.0F;
        }

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 4, "Rule M12.6.2 is not applied here as a macro is used")
#endif
        /* Set linewidth for Smooth mode */
        sgl_set_line_width(loc_f_pixel_width);
    }
    else {
        loc_f_half_pixel_width = 0.0F;
        loc_f_half_pixel_width_haloed = 0.0F;
    }

    sgl_draw_continuous_lines(loc_f_alpha_halo, loc_f_half_pixel_width_haloed, loc_f_half_pixel_width, loc_b_haloed_part, loc_b_colored_part);
    return;
}


void sgl_backup_for_polygon_smooth(sgl_polygon_smooth_backup * par_p_backup)
{
    par_p_backup->b_polygon_smooth = glob_pr_sglStatemachine->b_polygon_smooth;
    par_p_backup->b_stipple_ok = glob_pr_sglStatemachine->b_stipple_ok;
    par_p_backup->b_haloing_state = glob_pr_sglStatemachine->b_haloing_state;
    par_p_backup->tr_line_width = glob_pr_sglStatemachine->tr_line_width;
    par_p_backup->ul_nb_line_width = glob_pr_sglStatemachine->ul_nb_line_width;
    par_p_backup->l_linewidth_index = glob_pr_sglStatemachine->l_linewidth_index;
    par_p_backup->b_type = glob_pr_sglStatemachine->tr_geometry.b_type;

    /* Set line width to 2 for polygon smooth */
    glob_pr_sglStatemachine->b_polygon_smooth = SGL_FALSE;
    glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;
    glob_pr_sglStatemachine->b_haloing_state = SGL_FALSE;
    glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
    glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
    glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;

    if (glob_pr_sglStatemachine->b_draw_ellipse || glob_pr_sglStatemachine->b_draw_circle || glob_pr_sglStatemachine->b_draw_crown) {
        /* Use line width index 1 for circles and ellipses to have the best polygon smooth */
        glob_pr_sglStatemachine->l_linewidth_index = 1;
    }
    else {
        /* For shapes, Line width index 0 is enough and allows having straight edges */
        glob_pr_sglStatemachine->l_linewidth_index = 0;
    }
}

void sgl_restore_after_polygon_smooth(sgl_polygon_smooth_backup * par_p_backup)
{
    glob_pr_sglStatemachine->b_polygon_smooth = par_p_backup->b_polygon_smooth;
    glob_pr_sglStatemachine->b_stipple_ok = par_p_backup->b_stipple_ok;
    glob_pr_sglStatemachine->b_haloing_state = par_p_backup->b_haloing_state;
    glob_pr_sglStatemachine->tr_line_width = par_p_backup->tr_line_width;
    glob_pr_sglStatemachine->ul_nb_line_width = par_p_backup->ul_nb_line_width;
    glob_pr_sglStatemachine->l_linewidth_index = par_p_backup->l_linewidth_index;
    glob_pr_sglStatemachine->tr_geometry.b_type = par_p_backup->b_type;
}


/* End of File ***************************************************************/
