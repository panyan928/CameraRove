/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluCrown2f.c
 DESCRIPTION       : sgluCrown2f command shall draw a 2D Crown
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

/* Epsilon value for a crown radius */
#define SGL_EPSILON_RADIUS_CROWN    0.0001F
/* Epsilon value for a radius */
#define SGL_EPSILON_RADIUS		0.1F
/* Epsilon value for a positive float */
#define SGL_EPSILON_ABS			0.0001F


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_simple_outline_crown
  DESCRIPTION:
    Function shall draw the outline part of given crown without line stipple.
  SCOPE: global
  PARAMETERS:
    par_s_crown -> the description of the crown
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_simple_outline_crown(const sgl_crown * par_s_crown)
{
    SGLfloat loc_f_tmpx;
    SGLfloat loc_f_tmpy;
    SGLfloat loc_f_sin;
    SGLfloat loc_f_cos;
    SGLfloat loc_f_angle = par_s_crown->f_angle;
    SGLfloat loc_f_crown_angle = par_s_crown->f_angle_ok;
    SGLlong loc_l_m = par_s_crown->l_m;
    SGLlong loc_l_n = par_s_crown->l_n;

    sglBegin(SGL_LINE_LOOP);

    if (sgluFloatIsEqual(loc_f_crown_angle, 360.0F)) {
        /* Nothing to do */
    }
    else {
        sglVertex2f(par_s_crown->f_startinx, par_s_crown->f_startiny);
    }

    sglVertex2f(par_s_crown->f_startoutx, par_s_crown->f_startouty);

    while (loc_l_m < loc_l_n) {
        loc_f_angle = loc_f_angle + par_s_crown->f_angle_inc;
        mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
        loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusout * loc_f_cos);
        loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusout * loc_f_sin);

        sglVertex2f(loc_f_tmpx, loc_f_tmpy);
        loc_l_m++;
    }

    mth_cos_sin_degree(par_s_crown->f_angle_end, &loc_f_cos, &loc_f_sin);

    if (sgluFloatIsEqual(loc_f_crown_angle, 360.0F)) {
        sglEnd();

        sglBegin(SGL_LINE_LOOP);
    }
    else {
        /*
         ** Compute coordinate of the outside point
         */
        loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusout * loc_f_cos);
        loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusout * loc_f_sin);
        /*
         ** Set outside point
         */
        sglVertex2f(loc_f_tmpx, loc_f_tmpy);
    }
    /*
     ** Compute coordinate of the inside point
     */
    loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusin * loc_f_cos);
    loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusin * loc_f_sin);
    /*
     ** Set inside point
     */
    sglVertex2f(loc_f_tmpx, loc_f_tmpy);

    /* Loop for in arc-sector drawing */
    while (loc_l_m > 1L) {
        /*
         ** Calculate the next coordinate
         */
        mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
        loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusin * loc_f_cos);
        loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusin * loc_f_sin);
        /*
         ** Set the next point
         */
        sglVertex2f(loc_f_tmpx, loc_f_tmpy);

        /*
         ** Compute next angle
         */
        loc_l_m--;

        loc_f_angle = loc_f_angle - par_s_crown->f_angle_inc;
    }
    sglEnd();

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_filled_crown
  DESCRIPTION:
    Function shall draw the filled part of given crown.
  SCOPE: global
  PARAMETERS:
    par_s_crown -> the description of the crown
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_filled_crown(const sgl_crown * par_s_crown)
{
    SGLfloat loc_f_tmpx;
    SGLfloat loc_f_tmpy;
    SGLfloat loc_f_sin;
    SGLfloat loc_f_cos;
    SGLlong loc_l_n = par_s_crown->l_n;
    SGLfloat loc_f_angle = par_s_crown->f_angle;


    SGLbyte loc_b_draw_line_loop = 1U;
    SGLbyte loc_b_polygon_smooth_backup = glob_pr_sglStatemachine->b_polygon_smooth;
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;
    sgl_polygon_smooth_backup loc_struct_backup;

    if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
        if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
            loc_b_texture_alpha = SGL_TRUE;
        }
    }

    /* Backup polygon smooth, linestipple, haloing and linewidth */
    sgl_backup_for_polygon_smooth(&loc_struct_backup);

    if (loc_b_polygon_smooth_backup != SGL_TRUE) {
        loc_b_draw_line_loop = 0U;
    }
    else {
        /* Nothing to do */
    }

    if (glob_pr_sglStatemachine->b_transparency != SGL_MAX_ALPHA_8) {
        loc_b_draw_line_loop = 0U;
    }
    else {
        /* Nothing to do */
    }

    /* Treat polygon smooth by drawing first the outline */
    if (loc_b_draw_line_loop && (!(glob_pr_sglStatemachine->b_texture_state))) {
        sgl_draw_simple_outline_crown(par_s_crown);
    }
    else {
        /* Nothing to do */
    }

    if ((glob_pr_sglStatemachine->b_texture_state) && loc_b_texture_alpha && !glob_pr_sglStatemachine->b_modulate) {
        /* Nothing to do */
    }
    else {

        /* Update OGLX context to prepare a draw in SGL_TRIANGLE_STRIP mode, it is a filled mode */
        sgl_set_fill_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_TRIANGLE_STRIP;
        /* Set Geometry type to SGL_LINE_LOOP for surrounding line drawing of polygon smooth */
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        glob_pr_sglStatemachine->tr_geometry.l_size = 0;

        oglxVertex2f(par_s_crown->f_startinx, par_s_crown->f_startiny);
        oglxVertex2f(par_s_crown->f_startoutx, par_s_crown->f_startouty);

        while (loc_l_n > 1L) {
            loc_l_n--;
            loc_f_angle = loc_f_angle + par_s_crown->f_angle_inc;
            mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);

            loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusin * loc_f_cos);
            loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusin * loc_f_sin);
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);

            loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusout * loc_f_cos);
            loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusout * loc_f_sin);
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
        }

        if (par_s_crown->f_angle_ok >= 360.0F) {
            oglxVertex2f(par_s_crown->f_startinx, par_s_crown->f_startiny);
            oglxVertex2f(par_s_crown->f_startoutx, par_s_crown->f_startouty);
        }
        else {
            mth_cos_sin_degree(par_s_crown->f_angle_end, &loc_f_cos, &loc_f_sin);

            loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusin * loc_f_cos);
            loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusin * loc_f_sin);
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);

            loc_f_tmpx = par_s_crown->f_centerx + (par_s_crown->f_radiusout * loc_f_cos);
            loc_f_tmpy = par_s_crown->f_centery + (par_s_crown->f_radiusout * loc_f_sin);
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
        }

        sgl_end_shape();
    }
    glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
    /* Restore polygon smooth, linestipple, haloing and linewidth */
    sgl_restore_after_polygon_smooth(&loc_struct_backup);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglu_crown_2f_detect_error
  DESCRIPTION:
    Function shall check parameters of sgluCrown2f.
  SCOPE: global
  PARAMETERS:
    par_f_width -> width of the crown
    par_f_angle_in -> angle of the crown [0.0F,360.F]
    par_b_crown_value	-> crown mode (SGL_CROWN_FILLED or SGL_CROWN_OUTLINE)
  RETURN:
    SGLbyte -> Error detected (1U) or not (0U)
---------------------------------------------------------------------+*/
SGLbyte sglu_crown_2f_detect_error(SGLfloat par_f_width, SGLfloat par_f_radius, SGLbyte par_b_crown_value, SGLbyte par_b_clockwise)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check par_b_clockwise */
    if ((par_b_clockwise != SGL_TRUE) && (par_b_clockwise != SGL_FALSE)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (par_f_width > par_f_radius) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check width is positive */
    if (par_f_width <= 0.0F) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check crown kind is a normal one */
    if ((par_b_crown_value != SGL_CROWN_FILLED) && (par_b_crown_value != SGL_CROWN_OUTLINE)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglu_crown_2f
  DESCRIPTION:
    Function shall draw a 2D Crown.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center
    par_f_centery -> y-coordinate of the center
    par_f_startinx -> x-coordinate of the start
    par_f_startiny -> y-coordinate of the start
    par_f_width -> width of the crown
    par_b_clockwise -> clockwise or not
    par_f_angle_in -> angle of the crown [0.0F,360.F]
    par_b_crown_value	-> crown mode (SGL_CROWN_FILLED or SGL_CROWN_OUTLINE)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglu_crown_2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_radius, SGLfloat par_f_startinx, SGLfloat par_f_startiny,
                   SGLfloat par_f_width, SGLbyte par_b_clockwise, SGLfloat par_f_angle_in, SGLbyte par_b_crown_value)
{
    sgl_crown loc_s_sgl_crown;
    SGLfloat loc_f_tmpx = (par_f_startinx - par_f_centerx);
    SGLfloat loc_f_tmpy = (par_f_startiny - par_f_centery);
    SGLfloat loc_f_clockwise;
    SGLlong loc_l_m = 1L;
    SGLfloat loc_f_n;
    SGLlong loc_l_n;
    SGLfloat loc_f_angle;
    SGLfloat loc_f_angle_inc;
    SGLfloat loc_f_cos = 0.0F;
    SGLfloat loc_f_sin = 0.0F;

    SGLfloat loc_f_ratio = (glob_pr_sglStatemachine->f_ratio_scale_x + glob_pr_sglStatemachine->f_ratio_scale_y) * 0.5F;
    SGLfloat loc_f_radius = SGLfloat_div(par_f_radius, loc_f_ratio);
    /* If the radius is very small */
    if (loc_f_radius < MTH_RADIUS_VERY_SMALL_MAX) {
        /* Set the value of the angle increment to MTH_DELTA_I_VERY_SMALL */
        /* -- Radius very small */
        loc_f_angle_inc = MTH_DELTA_I_VERY_SMALL;
        /* For small radius, the delta index have to be smaller to get a
           better quality */
        loc_f_n = SGLfloat_div(par_f_angle_in, loc_f_angle_inc);
        loc_l_n = ((SGLlong) loc_f_n) + 1L;
    }
    /* Else */
    else {
        /* Compute the angle increment from the radius */
        loc_f_angle_inc = mth_get_angle_from_radius(loc_f_radius);
        /*
         ** Only if the radius superior at MTH_RADIUS_MEDIUM_MAX
         */
        loc_f_n = SGLfloat_div(par_f_angle_in, loc_f_angle_inc);

        /*
         ** Use The CEIL instead of the FLOOR
         */

        loc_l_n = mth_f_to_l_ceil(loc_f_n);
    }

    if (par_b_clockwise) {
        loc_f_clockwise = -1.0F;
    }
    else {
        loc_f_clockwise = 1.0F;
    }

    /* 
       loc_f_n is different from 0.0F as par_f_angle_in is different from 0.0F in computation of loc_f_n,
       it is ensured at the call of sglu_crown_2f.
     */
    loc_f_angle_inc = (SGLfloat_div(par_f_angle_in, loc_f_n) * loc_f_clockwise);

    loc_f_angle = mth_acos_degree(loc_f_tmpx, par_f_radius - par_f_width);
    loc_f_angle = mth_move_in_good_quadrant(loc_f_angle, loc_f_tmpx, loc_f_tmpy);

    loc_s_sgl_crown.f_angle = loc_f_angle;
    loc_s_sgl_crown.f_angle_end = loc_f_angle + (par_f_angle_in * loc_f_clockwise);
    loc_s_sgl_crown.f_angle_inc = loc_f_angle_inc;
    loc_s_sgl_crown.f_angle_ok = par_f_angle_in;
    loc_s_sgl_crown.b_clockwise = par_b_clockwise;
    loc_s_sgl_crown.f_centerx = par_f_centerx;
    loc_s_sgl_crown.f_centery = par_f_centery;
    loc_s_sgl_crown.f_radiusin = par_f_radius - par_f_width;
    loc_s_sgl_crown.f_radiusout = par_f_radius;
    loc_s_sgl_crown.f_startinx = par_f_startinx;
    loc_s_sgl_crown.f_startiny = par_f_startiny;
    mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
    loc_s_sgl_crown.f_startoutx = par_f_startinx + (par_f_width * loc_f_cos);
    loc_s_sgl_crown.f_startouty = par_f_startiny + (par_f_width * loc_f_sin);
    loc_s_sgl_crown.l_m = loc_l_m;
    loc_s_sgl_crown.l_n = loc_l_n;

    if (par_b_crown_value == SGL_CROWN_FILLED) {
        sgl_draw_filled_crown(&loc_s_sgl_crown);
    }
    else {
        /* par_b_crown_value == SGL_CROWN_OUTLINE */
        sgl_draw_simple_outline_crown(&loc_s_sgl_crown);
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluCrown2f
  DESCRIPTION:
    Function shall draw a 2D Crown.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center
    par_f_centery -> y-coordinate of the center
    par_f_radius -> radius of the crown
    par_f_width -> width of the crown
    par_f_start_angle -> start angle of the crown
    par_f_end_angle -> end angle of the crown
    par_b_clockwise -> clockwise orientation (SGL_TRUE or SGL_FALSE)
    par_b_crown_value	-> crown mode (SGL_CROWN_FILLED or SGL_CROWN_OUTLINE)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluCrown2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_radius, SGLfloat par_f_width,
                 SGLfloat par_f_start_angle, SGLfloat par_f_end_angle, SGLbool par_b_clockwise, SGLbyte par_b_crown_value)
{
    SGLbyte loc_b_error_detected = sglu_crown_2f_detect_error(par_f_width, par_f_radius, par_b_crown_value, par_b_clockwise);

    if (!loc_b_error_detected) {
        /* If width is equal to radius, an arc can be drawn */
        if (sgluFloatIsEqual(par_f_width, par_f_radius)) {
            if (par_b_crown_value == SGL_CROWN_FILLED) {
                sgluArcSector2f(par_f_centerx, par_f_centery, par_f_radius, par_f_start_angle, par_f_end_angle, par_b_clockwise, SGL_ARC_FILLED);
            }
            else {
                sgluArcSector2f(par_f_centerx, par_f_centery, par_f_radius, par_f_start_angle, par_f_end_angle, par_b_clockwise, SGL_ARC_OUTLINE);
            }
        }
        else {
            SGLfloat loc_f_angle;
            SGLfloat loc_f_start_x = (par_f_centerx + (par_f_radius * mth_cos_degree(par_f_start_angle)));
            SGLfloat loc_f_start_y = (par_f_centery + (par_f_radius * mth_sin_degree(par_f_start_angle)));

            SGLfloat loc_f_vector_x = (loc_f_start_x - par_f_centerx);
            SGLfloat loc_f_vector_y = (loc_f_start_y - par_f_centery);

            SGLfloat loc_f_startin_x = (par_f_centerx + (SGLfloat_div(loc_f_vector_x * (par_f_radius - par_f_width), par_f_radius)));
            SGLfloat loc_f_startin_y = (par_f_centery + (SGLfloat_div(loc_f_vector_y * (par_f_radius - par_f_width), par_f_radius)));

            SGLfloat loc_f_ratio = (glob_pr_sglStatemachine->f_ratio_scale_x + glob_pr_sglStatemachine->f_ratio_scale_y) * 0.5F;
            SGLfloat loc_f_radius = SGLfloat_div(par_f_radius, loc_f_ratio);

            if (loc_f_radius <= MTH_RADIUS_TOO_SMALL_MAX) {
                /* Draw a single point when radius is too small */
                sgl_vector2d loc_t_vertex_array[2];
                const void *loc_p_vertex_array;
                loc_t_vertex_array[0][0] = par_f_centerx;
                loc_t_vertex_array[0][1] = par_f_centery;

                loc_p_vertex_array = loc_t_vertex_array;

                oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);
#ifndef ES2_DEV_ENV
                glPointSize(loc_f_radius * 2.0F);
                oglxDrawVertexArray(GL_POINTS, 1, loc_p_vertex_array);
#else
                oglxDrawVertexArray_es2(GL_POINTS, 1, loc_p_vertex_array);
#endif
            }
            else {
                if (!par_b_clockwise) {
                    loc_f_angle = sgluModuloAngle(par_f_end_angle - par_f_start_angle);
                }
                else {
                    loc_f_angle = sgluModuloAngle(par_f_start_angle - par_f_end_angle);
                }

                if (!(sgluFloatIsEqual(loc_f_angle, 0.0F))) {
                    glob_pr_sglStatemachine->b_draw_crown = SGL_TRUE;
                    /* Disable texture coordinates computation in sglVertex2f */
                    sglu_crown_2f(par_f_centerx, par_f_centery, par_f_radius, loc_f_startin_x, loc_f_startin_y, par_f_width, par_b_clockwise, loc_f_angle,
                                  par_b_crown_value);
                    glob_pr_sglStatemachine->b_draw_crown = SGL_FALSE;
                }
                else {
                    /* Nothing to do */
                }
            }
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_CROWN2F, 0U);
    }
}


/* End of File ***************************************************************/
