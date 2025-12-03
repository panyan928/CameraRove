/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluArcCircleLinePointsList2f.c
 DESCRIPTION       : sgluArcCircleLinePointsList2f command shall generate list 
					of vertices for a 2D Arc
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

/* Float value of 360/PI */
#define SGL_360_OVER_PI			114.591F
/* Epsilon value for a radius */
#define SGL_EPSILON_RADIUS		0.1F
/* Epsilon value for a positive float */
#define SGL_EPSILON_ABS			0.0001F


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: ogl_simple_arc
  DESCRIPTION:
    Function shall compute list of vertices for arcs or 
    sectors.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
    par_f_clockwise -> clockwise mode (-1.0F or 1.0F)
    par_f_angle_ok -> angle of the arc or sector
    par_f_distance -> radius of the arc
	par_b_duplicate -> used to duplicate vertices in case of a stippled arc
  RETURN:
    None
---------------------------------------------------------------------+*/
void ogl_simple_arc(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx,
                    SGLfloat par_f_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius,
                    SGLbool par_b_duplicate)
{
    SGLlong loc_l_end_index = 1L;
    SGLfloat loc_f_tmpx = par_f_startx;
    SGLfloat loc_f_tmpy = par_f_starty;

    SGLfloat loc_f_deltax = (loc_f_tmpx - par_f_centerx);
    SGLfloat loc_f_deltay = (loc_f_tmpy - par_f_centery);

    SGLfloat loc_f_angle_end = ((loc_f_deltax * loc_f_deltax) + (loc_f_deltay * loc_f_deltay));
    /*
     ** radius calculation
     */
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_y;
    SGLfloat loc_f_sin = 0.0F;
    SGLfloat loc_f_cos = 0.0F;
    SGLfloat loc_f_sin_prev = 0.0F;
    SGLfloat loc_f_cos_prev = 0.0F;
    SGLfloat loc_f_sin_inc = 0.0F;
    SGLfloat loc_f_cos_inc = 0.0F;

    SGLfloat loc_f_distance = (SGLfloat_div(par_f_x_radius, loc_f_ratio_x) + SGLfloat_div(par_f_y_radius, loc_f_ratio_y)) * 0.5F;
    /*
     ** Local Variable ( set forward)
     */
    SGLlong loc_l_n;
    SGLfloat loc_f_n;
    SGLfloat loc_f_angle_inc;
    SGLfloat loc_f_angle;

    loc_f_angle = mth_acos_degree(loc_f_deltax, par_f_x_radius);

    if (loc_f_distance < MTH_RADIUS_VERY_SMALL_MAX) {
        loc_f_angle_inc = MTH_DELTA_I_VERY_SMALL;
        loc_f_n = SGLfloat_div(par_f_angle_ok, loc_f_angle_inc);
        loc_l_n = ((SGLlong) loc_f_n) + 1L;
    }
    else {
        /*
         ** Only if the radius superior to MTH_RADIUS_VERY_SMALL_MAX
         */
        loc_f_angle_inc = mth_get_angle_from_radius(loc_f_distance);
        loc_f_n = SGLfloat_div(par_f_angle_ok, loc_f_angle_inc);
        loc_l_n = mth_f_to_l_ceil(loc_f_n);
    }

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    if (loc_f_n > SGL_EPSILON_ABS) {
        loc_f_angle_inc = SGLfloat_div(par_f_angle_ok, loc_f_n) * par_f_clockwise;
    }
    else {
        loc_f_angle_inc = loc_f_angle_inc * par_f_clockwise;
    }
    loc_f_angle = mth_move_in_good_quadrant(loc_f_angle, loc_f_deltax, loc_f_deltay);

    loc_f_angle_end = loc_f_angle + (par_f_angle_ok * par_f_clockwise);

    /* If the arc is drawn after a call to sglVertex2f, there is no need to draw first vertex a second time */
    if ((!(glob_pr_sglStatemachine->b_new_vertex))
        || (par_b_duplicate)) {
        oglxVertex2f(par_f_startx, par_f_starty);
    }
    else {
        /* Nothing to do */
    }

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 4, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    /* Avoid last arc point too close from first point of next line */
    if ((loc_f_distance < MTH_RADIUS_MEDIUM_SMALL_MAX) && (glob_pr_sglStatemachine->b_drawing_mode == SGL_MODE_LINE)
        && (mth_f_abs(loc_f_angle) < 110.0F) && (!(glob_pr_sglStatemachine->b_draw_circle))) {
        loc_l_end_index = 2L;
    }
    else {
        /* Nothing to do */
    }
    mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
    mth_cos_sin_degree(loc_f_angle_inc, &loc_f_cos_inc, &loc_f_sin_inc);
    loc_f_sin_prev = loc_f_sin;
    loc_f_cos_prev = loc_f_cos;
    while (loc_l_n > loc_l_end_index) {
        loc_l_n--;
        loc_f_cos = (loc_f_cos_prev * loc_f_cos_inc) - (loc_f_sin_prev * loc_f_sin_inc);
        loc_f_sin = (loc_f_sin_prev * loc_f_cos_inc) + (loc_f_cos_prev * loc_f_sin_inc);
        loc_f_sin_prev = loc_f_sin;
        loc_f_cos_prev = loc_f_cos;
        loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
        loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);
        oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
        if (par_b_duplicate) {
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
        }
        else {
            /* Nothing to do */
        }
    }

    if ((par_f_angle_ok >= (SGLfloat) 360.0F) && !(glob_pr_sglStatemachine->b_close_line_loop)) {
        /* In the case of 360 degree, the last point is the first */
        oglxVertex2f(par_f_startx, par_f_starty);
    }
    else {
        /*
         ** Drawing the N-1 line
         ** Calculate the last coordinate for the last
         ** point
         */
        mth_cos_sin_degree(loc_f_angle_end, &loc_f_cos, &loc_f_sin);
        loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
        loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);

        /* Avoid drawing the last point if it is too close for the first point */
        if (glob_pr_sglStatemachine->b_close_line_loop) {
            if (((mth_f_abs(loc_f_tmpx - glob_pr_sglStatemachine->f_line_loop_first_vertex_x) > SGL_EPSILON_ABS)
                 || (mth_f_abs(loc_f_tmpy - glob_pr_sglStatemachine->f_line_loop_first_vertex_y) > SGL_EPSILON_ABS))) {
                /* Set the last point */
                oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
            }
        }
        else {
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
        }
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: ogl_compute_factor
  DESCRIPTION:
    Computes the factor between the radius correponding to given angle and the two radius of the ellipse.
  SCOPE: global
  PARAMETERS:
    par_f_a -> x radius of the ellipse
    par_f_b -> y radius of the ellipse
    par_f_angle -> current angle
  RETURN:
    the factor
---------------------------------------------------------------------+*/
SGLfloat ogl_compute_factor(SGLfloat par_f_a, SGLfloat par_f_b, SGLfloat par_f_angle)
{
    SGLfloat loc_f_factor = 1.0F;
    if (sgluFloatIsEqual(par_f_a, par_f_b)) {
        loc_f_factor = 1.0F;
    }
    else {
        SGLfloat loc_f_cos = mth_cos_degree(par_f_angle);
        SGLfloat loc_f_sin = mth_sin_degree(par_f_angle);
        SGLfloat loc_f_cos2 = loc_f_cos * loc_f_cos;
        SGLfloat loc_f_sin2 = loc_f_sin * loc_f_sin;
        SGLfloat loc_f_a2 = par_f_a * par_f_a;
        SGLfloat loc_f_b2 = par_f_b * par_f_b;
        SGLfloat loc_f_ab = par_f_a * par_f_b;
        SGLfloat loc_f_den = (loc_f_b2 * loc_f_cos2) + (loc_f_a2 * loc_f_sin2);
        SGLfloat loc_f_r;

        loc_f_den = mth_sqrtf(loc_f_den);
        loc_f_r = SGLfloat_div(loc_f_ab, loc_f_den);
        loc_f_factor = (SGLfloat_div(loc_f_r, par_f_a) + SGLfloat_div(loc_f_r, par_f_b)) * 0.5F;

        if (loc_f_factor > 3.0F) {
            loc_f_factor = 3.0F;
        }
    }
    return loc_f_factor;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: ogl_stippled_arc
  DESCRIPTION:
    Function shall compute list of vertices for arcs with linestippling.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
    par_f_clockwise -> clockwise mode (-1.0F or 1.0F)
    par_f_angle_ok -> angle of the arc or sector
    par_f_distance -> radius of the arc in pixels
    par_f_last_x -> to return X coordinate of last vertex
    par_f_last_y -> to return Y coordinate of last vertex
  RETURN:
    None
---------------------------------------------------------------------+*/
void ogl_stippled_arc(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx,
                      SGLfloat par_f_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius,
                      SGLfloat * par_f_last_x, SGLfloat * par_f_last_y)
{
    SGLfloat loc_f_tmpx = par_f_startx;
    SGLfloat loc_f_tmpy = par_f_starty;
    SGLfloat loc_f_tmpx2 = 0.0F;
    SGLfloat loc_f_tmpy2 = 0.0F;

    SGLfloat loc_f_deltax = (loc_f_tmpx - par_f_centerx);
    SGLfloat loc_f_deltay = (loc_f_tmpy - par_f_centery);

    SGLfloat loc_f_angle_inc = (((loc_f_deltax * loc_f_deltax) + (loc_f_deltay * loc_f_deltay)));

    SGLbool loc_b_start_stipple = SGL_TRUE;

    SGLfloat loc_f_last_x = 0.0F;
    SGLfloat loc_f_last_y = 0.0F;
    SGLfloat loc_f_norm = 0.0F;

    SGLlong loc_l_n;
    SGLlong loc_l_i;
    SGLfloat loc_f_angle;
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_y;

    SGLfloat loc_f_distance = (SGLfloat_div(par_f_x_radius, loc_f_ratio_x) + SGLfloat_div(par_f_y_radius, loc_f_ratio_y)) * 0.5F;

    SGLfloat loc_f_radiusmult2 = loc_f_distance * (SGLfloat) 2.0F;

    SGLfloat loc_f_a = SGLfloat_div(par_f_x_radius, loc_f_ratio_x);
    SGLfloat loc_f_b = SGLfloat_div(par_f_y_radius, loc_f_ratio_y);

    SGLfloat loc_f_angle_end;

    SGLfloat loc_tf_angle[SGL_MAX_STIPPLES_LENGTH];
    SGLbyte loc_tc_on_off[SGL_MAX_STIPPLES_LENGTH];
    SGLbyte loc_tc_small_point[SGL_MAX_STIPPLES_LENGTH];
    SGLfloat loc_f_sin = 0.0F;
    SGLfloat loc_f_cos = 0.0F;

    glob_pr_sglStatemachine->b_drawing_arc = SGL_TRUE;

    /* Initialize local stipple tables */
    for (loc_l_i = 0; loc_l_i < (SGLlong) SGL_MAX_STIPPLES_LENGTH; loc_l_i++) {
        loc_tf_angle[loc_l_i] = 0.0F;
        loc_tc_on_off[loc_l_i] = 0U;
        loc_tc_small_point[loc_l_i] = 0U;
    }

    /* Update arc angle */
    loc_f_angle = mth_acos_degree(loc_f_deltax, par_f_x_radius);

    loc_f_angle = mth_move_in_good_quadrant(loc_f_angle, loc_f_deltax, loc_f_deltay);

    loc_f_angle_end = loc_f_angle + (par_f_angle_ok * par_f_clockwise);

    /* Compute each angle of each stipple pattern */
    for (loc_l_n = 0L; loc_l_n < (SGLlong) (glob_pr_sglStatemachine->pr_stipple->b_number); loc_l_n++) {
        loc_tc_on_off[loc_l_n] = glob_pr_sglStatemachine->pr_stipple->tb_on_off[loc_l_n];

        if (glob_pr_sglStatemachine->pr_stipple->tb_length[loc_l_n] == (SGLbyte) 1) {
            loc_tc_small_point[loc_l_n] = SGL_TRUE;
        }
        else {
            loc_tc_small_point[loc_l_n] = SGL_FALSE;
        }

        loc_f_tmpx = SGLfloat_div((SGLfloat) glob_pr_sglStatemachine->pr_stipple->tb_length[loc_l_n], loc_f_radiusmult2);

        /* The range of parameter of mth_asin_radian is checked into the function */
        loc_f_tmpx = mth_asin_radian(loc_f_tmpx);
        loc_f_tmpx = (SGL_360_OVER_PI * loc_f_tmpx);

        loc_tf_angle[loc_l_n] = loc_f_tmpx * par_f_clockwise;
    }

    loc_l_n = (SGLlong) (glob_pr_sglStatemachine->ul_last_stipple_index);
    loc_f_angle_inc = loc_f_angle + loc_tf_angle[0U];

    /* The arc shall start at the first vertex */
    if (glob_pr_sglStatemachine->tr_geometry.l_size == 1) {
        glob_pr_sglStatemachine->tr_geometry.l_size--;
    }
    else {
        /* Nothing to do */
    }

    /* Compute loc_f_last_x and loc_f_last_y a first time in case no stipple is drawn. */
    mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
    loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
    loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);

    loc_f_last_x = loc_f_tmpx;
    loc_f_last_y = loc_f_tmpy;

    while ((loc_f_angle_inc * par_f_clockwise) < (loc_f_angle_end * par_f_clockwise)) {
        /* -- If it's the time to switch on the line */
        if (loc_tc_on_off[loc_l_n] == 1U) {
            SGLfloat loc_f_factor = 1.0F;
            SGLfloat loc_f_stipple_length = ((SGLfloat) (glob_pr_sglStatemachine->pr_stipple->tb_length[loc_l_n]));

            /* Calculate the first point of the pattern */
            mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
            loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
            loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);

            /* Calculate the Last point of the Full pattern */
            if (loc_tc_small_point[loc_l_n]) {
                /* In case of small stipple pattern : the first point is the
                   second too * like this : we have a point with good resolution */
                loc_f_tmpx2 = loc_f_tmpx;
                loc_f_tmpy2 = loc_f_tmpy;
            }
            else {
                SGLfloat loc_f_factor_angle = 1.0F;

                if (!loc_b_start_stipple) {
                    if (glob_pr_sglStatemachine->b_draw_ellipse) {
                        loc_f_factor_angle = ogl_compute_factor(loc_f_a, loc_f_b, loc_f_angle_inc);
                        loc_f_angle_inc = loc_f_angle + loc_f_factor_angle * loc_tf_angle[loc_l_n];
                        loc_f_factor = loc_f_factor_angle;
                    }

                    /* Normal drawing of the second point */

                    mth_cos_sin_degree(loc_f_angle_inc, &loc_f_cos, &loc_f_sin);
                    loc_f_tmpx2 = par_f_centerx + (par_f_x_radius * loc_f_cos);
                    loc_f_tmpy2 = par_f_centery + (par_f_y_radius * loc_f_sin);
                }
                else {
                    SGLfloat loc_f_tmp_angle = SGLfloat_div(loc_f_stipple_length - glob_pr_sglStatemachine->f_last_stipple_length, loc_f_radiusmult2);

                    if (loc_f_tmp_angle > (SGLfloat) 1.0F) {
                        loc_f_tmp_angle = 180.0F;
                    }
                    else {
                        loc_f_tmp_angle = mth_asin_radian(loc_f_tmp_angle);
                        loc_f_tmp_angle = (SGL_360_OVER_PI * loc_f_tmp_angle) * par_f_clockwise;
                    }
                    loc_f_angle_inc = loc_f_angle + loc_f_tmp_angle;
                    /* Normal drawing of the second point */

                    if (glob_pr_sglStatemachine->b_draw_ellipse) {
                        loc_f_factor_angle = ogl_compute_factor(loc_f_a, loc_f_b, loc_f_angle_inc);
                        loc_f_angle_inc = loc_f_angle + loc_f_factor_angle * loc_tf_angle[loc_l_n];
                        loc_f_factor = loc_f_factor_angle;
                    }
                    else {
                        if (loc_f_stipple_length > glob_pr_sglStatemachine->f_last_stipple_length) {
                            loc_f_factor = SGLfloat_div(loc_f_stipple_length - glob_pr_sglStatemachine->f_last_stipple_length, loc_f_stipple_length);
                        }
                    }

                    mth_cos_sin_degree(loc_f_angle_inc, &loc_f_cos, &loc_f_sin);
                    loc_f_tmpx2 = par_f_centerx + (par_f_x_radius * loc_f_cos);
                    loc_f_tmpy2 = par_f_centery + (par_f_y_radius * loc_f_sin);
                }
            }

            ogl_simple_arc(par_f_centerx, par_f_centery, loc_f_tmpx, loc_f_tmpy, par_f_clockwise, loc_tf_angle[loc_l_n] * par_f_clockwise * loc_f_factor,
                           par_f_x_radius, par_f_y_radius, SGL_TRUE);

        }
        else {
            if (loc_b_start_stipple) {
                SGLfloat loc_f_tmp_angle =
                    SGLfloat_div(((SGLfloat) glob_pr_sglStatemachine->pr_stipple->tb_length[loc_l_n] - glob_pr_sglStatemachine->f_last_stipple_length),
                                 loc_f_radiusmult2);

                if (loc_f_tmp_angle > (SGLfloat) 1.0F) {
                    loc_f_tmp_angle = 180.0F;
                }
                else {
                    loc_f_tmp_angle = mth_asin_radian(loc_f_tmp_angle);
                    loc_f_tmp_angle = (SGL_360_OVER_PI * loc_f_tmp_angle) * par_f_clockwise;
                }

                loc_f_angle_inc = loc_f_angle + loc_f_tmp_angle;
            }
            else {
                /* Nothing to do */
            }
        }

        loc_f_last_x = loc_f_tmpx2;
        loc_f_last_y = loc_f_tmpy2;

        /* Next pattern */
        loc_l_n++;
        if ((loc_l_n + 1L) > (SGLlong) (glob_pr_sglStatemachine->pr_stipple->b_number)) {
            /* The next pattern is the first */
            loc_l_n = 0L;
        }
        else {
            /* Nothing to do */
        }
        /* Set the next angle */
        loc_f_angle = loc_f_angle_inc;
        /* Set the next limit */
        if (glob_pr_sglStatemachine->b_draw_ellipse) {
            SGLfloat loc_f_factor_angle = ogl_compute_factor(loc_f_a, loc_f_b, loc_f_angle_inc);
            loc_f_angle_inc = loc_f_angle + loc_f_factor_angle * loc_tf_angle[loc_l_n];
        }
        else {
            loc_f_angle_inc = loc_f_angle + loc_tf_angle[loc_l_n];
        }

        loc_b_start_stipple = SGL_FALSE;
    }
    /* Draw the last segment to the last point if it is ON
       Because the increment is not exactly the last point */
    if (loc_tc_on_off[loc_l_n] == 1U) {
        mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
        loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
        loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);

        loc_f_last_x = loc_f_tmpx;
        loc_f_last_y = loc_f_tmpy;

        if (loc_tc_small_point[loc_l_n]) {
            loc_f_tmpx2 = loc_f_tmpx;
            loc_f_tmpy2 = loc_f_tmpy;
        }
        else {
            mth_cos_sin_degree(loc_f_angle_end, &loc_f_cos, &loc_f_sin);
            loc_f_tmpx2 = par_f_centerx + (par_f_x_radius * loc_f_cos);
            loc_f_tmpy2 = par_f_centery + (par_f_y_radius * loc_f_sin);
        }

        ogl_simple_arc(par_f_centerx, par_f_centery, loc_f_tmpx, loc_f_tmpy, par_f_clockwise, (loc_f_angle_end - loc_f_angle) * par_f_clockwise,
                       par_f_x_radius, par_f_y_radius, SGL_TRUE);

    }
    else {
        mth_cos_sin_degree(loc_f_angle_end, &loc_f_cos, &loc_f_sin);
        loc_f_tmpx2 = par_f_centerx + (par_f_x_radius * loc_f_cos);
        loc_f_tmpy2 = par_f_centery + (par_f_y_radius * loc_f_sin);
    }

    glob_pr_sglStatemachine->ul_last_stipple_index = (SGLulong) loc_l_n;

    loc_f_norm = mth_norm(loc_f_last_x, loc_f_last_y, loc_f_tmpx2, loc_f_tmpy2);
    glob_pr_sglStatemachine->f_last_stipple_length = SGLfloat_div(loc_f_norm, (glob_pr_sglStatemachine->f_ratio_x + glob_pr_sglStatemachine->f_ratio_y) * 0.5F);

    /* Return coordinates of the last vertex */
    *par_f_last_x = loc_f_tmpx;
    *par_f_last_y = loc_f_tmpy;
    glob_pr_sglStatemachine->b_drawing_arc = SGL_FALSE;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: ogl_sglu_compute_arc_distance
  DESCRIPTION:
    Function shall compute the radius of an arc
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
  RETURN:
    SGLfloat -> Radius of the arc
---------------------------------------------------------------------+*/
SGLfloat ogl_sglu_compute_arc_distance(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_y;

    SGLfloat loc_f_distance =
        ((par_f_startx - par_f_centerx) * (par_f_startx - par_f_centerx)) + ((par_f_starty - par_f_centery) * (par_f_starty - par_f_centery));
    loc_f_distance = mth_sqrtf(loc_f_distance);

    loc_f_distance = (SGLfloat_div(loc_f_distance, loc_f_ratio_x) + SGLfloat_div(loc_f_distance, loc_f_ratio_y)) * 0.5F;

    if (loc_f_distance < SGL_EPSILON_RADIUS) {
        loc_f_distance = SGL_EPSILON_RADIUS;
    }
    else {
        /* Nothing to do */
    }

    return loc_f_distance;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: ogl_sglu_arc_line_loop_strip
  DESCRIPTION:
    Function shall generate list of vertices for a 2D Arc defined as line loop or line strip.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
    par_f_calculated_startx -> calculated x starting point
    par_f_calculated_starty -> calculated y starting point
    par_f_clockwise -> orientation 
    par_f_angle_ok -> angle in degrees of the arc or sector
	par_f_distance -> radius of the arc
	par_f_last_pointx -> x coordinate of last vertex of the arc
	par_f_last_pointy -> y coordinate of last vertex of the arc
  RETURN:
    None  
---------------------------------------------------------------------+*/
void ogl_sglu_arc_line_loop_strip(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                  SGLfloat par_f_calculated_startx, SGLfloat par_f_calculated_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok,
                                  SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLfloat * par_f_last_pointx, SGLfloat * par_f_last_pointy)
{
    /* 
       A line stipple exists which consists of only one dot ON. This stipple
       is treated as a "no stipple". To have the same behaviour on OGLX, here the stipple is
       analysed to detect whether the stipple contains only ON pixels and enables the stipple
       only if this is not the case.

     */
    SGLbyte loc_b_draw_stippled = 1U;

    if (glob_pr_sglStatemachine->b_stipple_ok != SGL_TRUE) {
        loc_b_draw_stippled = 0U;
    }
    else {
        /* Nothing to do */
    }

    if (loc_b_draw_stippled != 0U) {
        SGLbyte loc_b_close_line_loop_backup;


        /* origin of the arcs is given start point */
        if (!(glob_pr_sglStatemachine->b_simple_arc)) {
            oglxVertex2f(par_f_startx, par_f_starty);
        }
        else {
            /* Nothing to do */
        }

        /* Save first vertex of the line loop to close it */
        if (glob_pr_sglStatemachine->ul_currentAction == SGL_LINE_LOOP) {
            /* Select the mode SGL_LINE_STRIP */
            glob_pr_sglStatemachine->ul_currentAction = SGL_LINE_STRIP;
        }
        else {
            /* Nothing to do */
        }

        /* To avoid to close the line loop during arcs drawing,
           save the flag in a local variable and set it to OFF */
        loc_b_close_line_loop_backup = glob_pr_sglStatemachine->b_close_line_loop;
        glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
        glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;

        glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;

        glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_FALSE;

        /* Send to the cutting treatment */
        /* Call the function ge_arc_circle_line_points_list_stipple */
        ogl_stippled_arc(par_f_centerx, par_f_centery, par_f_calculated_startx, par_f_calculated_starty,
                         par_f_clockwise, par_f_angle_ok, par_f_x_radius, par_f_y_radius, par_f_last_pointx, par_f_last_pointy);

        glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;
        /* reset the switch of line mode */

        /* Restore the flag */
        glob_pr_sglStatemachine->b_close_line_loop = loc_b_close_line_loop_backup;

        /* Go on with line strip the line loop is closed if necessary thanks to 
           the test on glob_pr_sglStatemachine->b_close_line_loop */

        if (par_f_angle_ok < 360.0F) {
            oglxVertex2f(*par_f_last_pointx, *par_f_last_pointy);
        }
        else {
            /* Nothing to do */
        }

        glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_TRUE;
    }
    else {
        ogl_simple_arc(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_clockwise, par_f_angle_ok, par_f_x_radius, par_f_y_radius, SGL_FALSE);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: ogl_sglu_arc_lines
  DESCRIPTION:
    Function shall generate list of vertices for a 2D Arc defined as lines.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
    par_f_clockwise -> orientation 
    par_f_angle_ok -> angle in degrees of the arc or sector
	par_f_distance -> radius of the arc
	par_f_last_pointx -> x coordinate of last vertex of the arc
	par_f_last_pointy -> y coordinate of last vertex of the arc
  RETURN:
    None  
---------------------------------------------------------------------+*/
void ogl_sglu_arc_lines(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                        SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLfloat * par_f_last_pointx,
                        SGLfloat * par_f_last_pointy)
{
    SGLbyte loc_b_draw_stippled = 1U;

    if (glob_pr_sglStatemachine->b_stipple_ok != SGL_TRUE) {
        loc_b_draw_stippled = 0U;
    }
    else {
        /* Nothing to do */
    }

    if (loc_b_draw_stippled != 0U) {
        glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;

        glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_FALSE;

        ogl_stippled_arc(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_clockwise,
                         par_f_angle_ok, par_f_x_radius, par_f_y_radius, par_f_last_pointx, par_f_last_pointy);
        glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;

        glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_TRUE;
    }
    else {

        sglEnd();
        sglBegin(SGL_LINE_STRIP);
        {
            ogl_simple_arc(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_clockwise, par_f_angle_ok, par_f_x_radius, par_f_y_radius,
                           SGL_FALSE);
        }
        sglEnd();
        sglBegin(SGL_LINES);

    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluArcCircleLinePointsList2f
  DESCRIPTION:
    Function shall generate a list of vertices for a 2D Arc.
  SCOPE: global
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
    par_b_clockwise -> orientation of angle
    par_f_angle -> angle in degrees of the arc or sector
  RETURN:
    None  
---------------------------------------------------------------------+*/
void sgluArcCircleLinePointsList2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                   SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLbyte par_b_clockwise, SGLfloat par_f_angle)
{
    SGLfloat loc_f_clockwise;
    SGLfloat loc_f_angle_ok;

    /* Local variables used to calculate the arc radius */
    SGLfloat loc_f_distance;

    /* Coordinates of the last point of the arc drawn */
    SGLfloat loc_f_last_pointx = 0.0F;
    SGLfloat loc_f_last_pointy = 0.0F;

    /* Starting point for angles greater or equal to 360 degrees */
    SGLfloat loc_f_calculated_startx = par_f_startx;
    SGLfloat loc_f_calculated_starty = par_f_starty;

    SGLbyte loc_b_error_detected = 0U;

    if (par_b_clockwise) {
        loc_f_clockwise = -1.0F;
    }
    else {
        loc_f_clockwise = 1.0F;
    }


    if (par_f_angle >= 360.0F) {
        loc_f_angle_ok = 360.0F;

        /* In order to close the line loop avoiding a problem of precision
           when calculating the vertices to draw. */
        if (glob_pr_sglStatemachine->ul_currentAction == SGL_LINE_LOOP) {
            /* Calculate radius */
            loc_f_distance =
                -mth_sqrtf(((par_f_startx - par_f_centerx) * (par_f_startx - par_f_centerx)) +
                           ((par_f_starty - par_f_centery) * (par_f_starty - par_f_centery)));

            /* Use as start point the last point in the negative part of the horizontal diameter */
            loc_f_calculated_startx = loc_f_distance + par_f_centerx;
            loc_f_calculated_starty = par_f_centery;
        }
        /* Else do nothing */
        else {
            /* Nothing to do */
        }
    }
    else {
        loc_f_angle_ok = par_f_angle;
    }

    /* Check angle is positive */
    if (loc_f_angle_ok <= (SGLfloat) 0.0F) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        loc_f_distance = ogl_sglu_compute_arc_distance(par_f_centerx, par_f_centery, par_f_startx, par_f_starty);
        if (loc_f_distance <= MTH_RADIUS_TOO_SMALL_MAX) {
            oglxVertex2f(par_f_centerx, par_f_centery);
            oglxVertex2f(par_f_centerx, par_f_centery);
        }
        else {


            switch (glob_pr_sglStatemachine->ul_currentAction) {

            case SGL_LINES:
            {
                ogl_sglu_arc_lines(par_f_centerx, par_f_centery, par_f_startx, par_f_starty,
                                   loc_f_clockwise, loc_f_angle_ok, par_f_x_radius, par_f_y_radius, &loc_f_last_pointx, &loc_f_last_pointy);
            }
                break;
            case SGL_LINE_STRIP:
            case SGL_LINE_LOOP:
            {
                ogl_sglu_arc_line_loop_strip(par_f_centerx, par_f_centery, par_f_startx, par_f_starty,
                                             loc_f_calculated_startx, loc_f_calculated_starty, loc_f_clockwise, loc_f_angle_ok, par_f_x_radius, par_f_y_radius,
                                             &loc_f_last_pointx, &loc_f_last_pointy);
            }
                break;
            default:
                /*
                   case SGL_TRIANGLE_STRIP:
                   case SGL_POLYGON:
                   case SGL_STENCIL1:
                   case SGL_STENCIL2:
                 */
            {
                ogl_simple_arc(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, loc_f_clockwise, loc_f_angle_ok, par_f_x_radius, par_f_y_radius,
                               SGL_FALSE);
            }
                break;

            }
        }
    }
    else {
        /* Nothing to do */
    }
    return;
}

/* End of File ***************************************************************/
