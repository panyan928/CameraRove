/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglShapeActiveAreaGetStatus.c
 DESCRIPTION       : sglShapeActiveAreaGetStatus command shall return the status 
					of the mouse position: SGL_TRUE shall be returned if the mouse 
					is inside the shape active area, SGL_FALSE shall be returned if 
					the mouse is outside the shape active area
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

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

#define LOC_EPSILON 0.01F

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_min_value
  DESCRIPTION:
    Function shall return the minimum of the two floats.
  SCOPE: global
  PARAMETERS:  
    par_f_value1 -> value 1 to compare
    par_f_value2 -> value 2 to compare
  RETURN:
    SGLfloat -> minimum value of the two floats  
---------------------------------------------------------------------+*/
SGLfloat oglx_min_value(SGLfloat par_f_value1, SGLfloat par_f_value2)
{
    SGLfloat loc_f_return;

    if (par_f_value1 > par_f_value2) {
        loc_f_return = par_f_value2;
    }
    else {
        loc_f_return = par_f_value1;
    }

    return loc_f_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_max_value
  DESCRIPTION:
    Function shall return the maximum of the two floats.
  SCOPE: global
  PARAMETERS:  
    par_f_value1 -> value 1 to compare
    par_f_value2 -> value 2 to compare	
  RETURN:
    SGLfloat -> maximum value of the two floats  
---------------------------------------------------------------------+*/
SGLfloat oglx_max_value(SGLfloat par_f_value1, SGLfloat par_f_value2)
{
    SGLfloat loc_f_return;

    if (par_f_value1 < par_f_value2) {
        loc_f_return = par_f_value2;
    }
    else {
        loc_f_return = par_f_value1;
    }

    return loc_f_return;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 25, "Rule M16.7 is not applied here as function declaration is compliant with IRS SGL")
#pragma attol crc_justify_all (Rule M8.3, 25, "Rule M8.3 is not applied here as it does not work on table parameters, it is a RTRT CR problem")
#endif
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglShapeActiveAreaGetStatus
  DESCRIPTION:
    Function shall return the status of the mouse position: 
    SGL_TRUE shall be returned if the mouse is inside the shape 
    active area, SGL_FALSE shall be returned if the mouse is 
    outside the shape active area.
  SCOPE: global
  PARAMETERS:  
    par_f_mouse_x -> x-coordinate for mouse
    par_f_mouse_y -> y-coordinate for mouse
    par_l_nb_points -> number of points of the shape
    par_pf_points -> Points of the shape
  RETURN:
    SGLbyte -> SGL_TRUE: the mouse is in the shape,
    SGL_FALSE: the mouse is out of the shape
---------------------------------------------------------------------+*/
SGLbool sglShapeActiveAreaGetStatus(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLlong par_l_nb_points, SGLfloat par_pf_points[][2])
{
    SGLbyte loc_b_return = SGL_FALSE;
    SGLbyte loc_b_inside_scissors = SGL_TRUE;

    SGLlong loc_l_counter = 0L;
    SGLlong loc_l_i;

    SGLfloat loc_tf_p1[2];
    SGLfloat loc_tf_p2[2];
    SGLfloat loc_f_last_intersection = 0.0F;
    SGLfloat loc_f_first_intersection = 0.0F;
    SGLbool loc_b_intersect_first = SGL_FALSE;
    SGLfloat loc_f_mouse_y;

    par_f_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    par_f_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;
    loc_f_mouse_y = par_f_mouse_y;

    /* Convert the position in pixel coordinate system */
    loc_tf_p1[0]
        = (glob_pr_sglStatemachine->f_modelview_matrix[0] * par_pf_points[0][0]
           + glob_pr_sglStatemachine->f_modelview_matrix[4] * par_pf_points[0][1]
           + glob_pr_sglStatemachine->f_modelview_matrix[12] + 1.0F) * glob_pr_sglStatemachine->f_viewport_width_div_2 + glob_pr_sglStatemachine->f_viewport_x1;

    loc_tf_p1[1]
        = (glob_pr_sglStatemachine->f_modelview_matrix[1] * par_pf_points[0][0]
           + glob_pr_sglStatemachine->f_modelview_matrix[5] * par_pf_points[0][1]
           + glob_pr_sglStatemachine->f_modelview_matrix[13] + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2 +
        glob_pr_sglStatemachine->f_viewport_y1;
    /*
       Ray casting algorithm:
       Consider a polygon made up of N vertices (xi,yi) where i ranges from 0 to N-1. The last vertex (xN,yN) is assumed to be the same as the first vertex (x0,y0), 
       that is, the polygon is closed. To determine the status of a point (xp,yp) consider a horizontal ray emanating from (xp,yp) and to the right. 
       If the number of times this ray intersects the line segments making up the polygon is even then the point is outside the polygon. Whereas if the number of 
       intersections is odd then the point (xp,yp) lies inside the polygon.
     */

    for (loc_l_i = 1; loc_l_i <= par_l_nb_points; loc_l_i++) {
        par_f_mouse_y = loc_f_mouse_y;

        loc_tf_p2[0]
            = (glob_pr_sglStatemachine->f_modelview_matrix[0] * par_pf_points[loc_l_i % par_l_nb_points][0]
               + glob_pr_sglStatemachine->f_modelview_matrix[4] * par_pf_points[loc_l_i % par_l_nb_points][1]
               + glob_pr_sglStatemachine->f_modelview_matrix[12] + 1.0F) * glob_pr_sglStatemachine->f_viewport_width_div_2 +
            glob_pr_sglStatemachine->f_viewport_x1;

        loc_tf_p2[1]
            = (glob_pr_sglStatemachine->f_modelview_matrix[1] * par_pf_points[loc_l_i % par_l_nb_points][0]
               + glob_pr_sglStatemachine->f_modelview_matrix[5] * par_pf_points[loc_l_i % par_l_nb_points][1]
               + glob_pr_sglStatemachine->f_modelview_matrix[13] + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2 +
            glob_pr_sglStatemachine->f_viewport_y1;

        if (sgluFloatIsEqual(par_f_mouse_y, loc_tf_p1[1]) || sgluFloatIsEqual(par_f_mouse_y, loc_tf_p2[1])) {
            par_f_mouse_y = par_f_mouse_y + LOC_EPSILON;
        }

        if (par_f_mouse_y > oglx_min_value(loc_tf_p1[1], loc_tf_p2[1])) {
            if (par_f_mouse_y < oglx_max_value(loc_tf_p1[1], loc_tf_p2[1])) {
                if (par_f_mouse_x <= oglx_max_value(loc_tf_p1[0], loc_tf_p2[0])) {
                    if (((loc_tf_p2[1] - loc_tf_p1[1]) > SGL_MIN_F)
                        || ((loc_tf_p2[1] - loc_tf_p1[1]) < -SGL_MIN_F)) {
                        SGLfloat loc_f_x_intersection;

                        loc_f_x_intersection = SGLfloat_div(((par_f_mouse_y - loc_tf_p1[1]) * (loc_tf_p2[0] - loc_tf_p1[0]))
                                                            , (loc_tf_p2[1] - loc_tf_p1[1])) + loc_tf_p1[0];

                        if ((((loc_tf_p2[0] - loc_tf_p1[0]) < SGL_MIN_F)
                             && ((loc_tf_p2[0] - loc_tf_p1[0]) > -SGL_MIN_F))
                            || (par_f_mouse_x <= loc_f_x_intersection)) {
                            if (loc_l_counter == 0) {
                                loc_l_counter++;
                                loc_f_last_intersection = loc_f_x_intersection;
                                if (loc_l_i == 1) {
                                    loc_f_first_intersection = loc_f_x_intersection;
                                    loc_b_intersect_first = SGL_TRUE;
                                }
                            }
                            else {
                                if (loc_l_i == par_l_nb_points) {
                                    /* Check intersection with first segment and last segment */
                                    if (loc_b_intersect_first) {
                                        if (!sgluFloatIsEqual(loc_f_first_intersection, loc_f_x_intersection)
                                            && !sgluFloatIsEqual(loc_f_last_intersection, loc_f_x_intersection)) {
                                            loc_l_counter++;
                                            loc_f_last_intersection = loc_f_x_intersection;
                                        }
                                    }
                                    else {
                                        if (!sgluFloatIsEqual(loc_f_last_intersection, loc_f_x_intersection)) {
                                            loc_l_counter++;
                                            loc_f_last_intersection = loc_f_x_intersection;
                                        }
                                    }
                                }
                                else {
                                    /* Check this intersection is not the same as last one */
                                    if (!sgluFloatIsEqual(loc_f_last_intersection, loc_f_x_intersection)) {
                                        loc_l_counter++;
                                        loc_f_last_intersection = loc_f_x_intersection;
                                    }
                                }
                            }
                        }
                        else {
                            /* Nothing to do */
                        }
                    }
                    else {
                        /* Nothing to do */
                    }
                }
                else {
                    /* Nothing to do */
                }
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }

        loc_tf_p1[0] = loc_tf_p2[0];
        loc_tf_p1[1] = loc_tf_p2[1];
    }

    if ((loc_l_counter % 2) != 0) {
        loc_b_return = SGL_TRUE;
    }
    else {
        loc_b_return = SGL_FALSE;
    }

    if (loc_b_return) {
        loc_b_inside_scissors = sgl_pixel_point_is_inside_scissors(par_f_mouse_x, par_f_mouse_y);
        if (loc_b_inside_scissors != SGL_TRUE) {
            loc_b_return = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}

/* End of File ***************************************************************/
