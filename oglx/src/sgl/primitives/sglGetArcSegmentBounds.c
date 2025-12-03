/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetArcSegmentBounds.c
 DESCRIPTION       : sglGetArcSegmentBounds command shall return the bounds of an arc segment
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
#endif

#define OGLX_NE 0
#define OGLX_NW 1
#define OGLX_SW 2
#define OGLX_SE 3

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_angle_position
  DESCRIPTION:
    Function shall return the position of the given angle.
  SCOPE: global
  PARAMETERS:
	par_f_angle -> Input angle
  RETURN:
    The position: OGLX_NE if par_f_angle is in the range [0,90[
				OGLX_NW if par_f_angle is in the range [90,180[
				OGLX_SW if par_f_angle is in the range [180,270[
				OGLX_SE otherwise.
---------------------------------------------------------------------+*/
SGLulong oglx_get_angle_position(SGLfloat par_f_angle)
{
    SGLulong loc_ul_return;

    if (par_f_angle < 90.0F) {
        loc_ul_return = OGLX_NE;
    }
    else {
        if (par_f_angle < 180.0F) {
            loc_ul_return = OGLX_NW;
        }
        else {
            if (par_f_angle < 270.0F) {
                loc_ul_return = OGLX_SW;
            }
            else {
                loc_ul_return = OGLX_SE;
            }
        }
    }

    return loc_ul_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_get_arc_segment_bounds_detect_error
  DESCRIPTION:
    Function shall detect errors on sglGetArcSegmentBounds.
  SCOPE: global
  PARAMETERS:
    par_b_clockwise -> Orientation of segment
    par_f_angle -> Angle of segment
  RETURN:
    SGLbool -> SGL_TRUE if an error is detected, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_get_arc_segment_bounds_detect_error(SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    SGLbool loc_b_return = SGL_FALSE;
    SGLfloat loc_f_angle = sgluModuloAngle(par_f_angle);

    if ((par_b_clockwise != SGL_TRUE) && (par_b_clockwise != SGL_FALSE)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (sgluFloatIsEqual(loc_f_angle, 0.0F)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (sgluFloatIsEqual(loc_f_angle, 360.0F)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetArcSegmentBounds
  DESCRIPTION:
    Function shall return the bounds of an arc segment.
  SCOPE: global
  PARAMETERS:
    par_b_clockwise -> orientation of the arc
	par_f_angle -> Angle of the arc
    par_f_x1 -> x coordinate of start point
    par_f_y1 -> y coordinate of start point
    par_f_x2 -> x coordinate of end point
    par_f_y2 -> y coordinate of end point
    par_pf_x1 -> x coordinate of lower left corner of the bounding box
    par_pf_y1 -> y coordinate of lower left corner of the bounding box
    par_pf_x2 -> x coordinate of upper right corner of the bounding box
    par_pf_y2 -> y coordinate of upper right corner of the bounding box
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglGetArcSegmentBounds(SGLbool par_b_clockwise, SGLfloat par_f_angle,
                            SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2,
                            SGLfloat * par_pf_x1, SGLfloat * par_pf_y1, SGLfloat * par_pf_x2, SGLfloat * par_pf_y2)
{
    SGLbool loc_b_error_detected = sgl_get_arc_segment_bounds_detect_error(par_b_clockwise, par_f_angle);

    if (loc_b_error_detected) {
        *par_pf_x1 = 0.0F;
        *par_pf_y1 = 0.0F;
        *par_pf_x2 = 0.0F;
        *par_pf_y2 = 0.0F;
        oglxSetError(SGL_ERROR_SGL_GETARCSEGMENTBOUNDS, 0U);
    }
    else {
        SGLfloat loc_f_angle = par_f_angle;
        SGLfloat loc_f_beta_angle = (180.0F - loc_f_angle) * 0.5F;
        SGLfloat loc_f_x1 = par_f_x1;
        SGLfloat loc_f_y1 = par_f_y1;
        SGLfloat loc_f_x2 = par_f_x2;
        SGLfloat loc_f_y2 = par_f_y2;
        SGLfloat loc_f_vector_x = (loc_f_x2 - loc_f_x1);
        SGLfloat loc_f_vector_y = (loc_f_y2 - loc_f_y1);
        SGLfloat loc_f_x_square = (loc_f_vector_x * loc_f_vector_x);
        SGLfloat loc_f_y_square = (loc_f_vector_y * loc_f_vector_y);
        SGLfloat loc_f_norm = mth_sqrtf(loc_f_x_square + loc_f_y_square);
        SGLfloat loc_f_radius = SGLfloat_div(loc_f_norm, 2.0f * mth_cos_degree(loc_f_beta_angle));
        SGLfloat loc_f_middle_x = ((loc_f_x2 + loc_f_x1) * 0.5F);
        SGLfloat loc_f_middle_y = ((loc_f_y2 + loc_f_y1) * 0.5F);
        SGLbool loc_b_north = SGL_FALSE;
        SGLbool loc_b_south = SGL_FALSE;
        SGLbool loc_b_east = SGL_FALSE;
        SGLbool loc_b_west = SGL_FALSE;
        SGLfloat loc_f_min_x;
        SGLfloat loc_f_min_y;
        SGLfloat loc_f_max_x;
        SGLfloat loc_f_max_y;
        SGLfloat loc_pf_x[2];
        SGLfloat loc_pf_y[2];

        SGLfloat loc_f_orientation;
        loc_pf_x[0] = par_f_x1;
        loc_pf_x[1] = par_f_x2;
        loc_pf_y[0] = par_f_y1;
        loc_pf_y[1] = par_f_y2;

        loc_f_min_x = mth_get_min_value(2, loc_pf_x);
        loc_f_min_y = mth_get_min_value(2, loc_pf_y);
        loc_f_max_x = mth_get_max_value(2, loc_pf_x);
        loc_f_max_y = mth_get_max_value(2, loc_pf_y);

        if (par_b_clockwise) {
            loc_f_orientation = -1.0F;
        }
        else {
            loc_f_orientation = 1.0F;
        }

        if (loc_f_norm > SGL_MIN_F) {
            SGLfloat loc_f_arc_center_x =
                (loc_f_middle_x - (SGLfloat_div((loc_f_orientation * (loc_f_radius * mth_cos_degree(loc_f_angle * 0.5F))) * loc_f_vector_y, loc_f_norm)));
            SGLfloat loc_f_arc_center_y =
                (loc_f_middle_y + (SGLfloat_div((loc_f_orientation * (loc_f_radius * mth_cos_degree(loc_f_angle * 0.5F))) * loc_f_vector_x, loc_f_norm)));

            SGLfloat loc_f_deltax = (par_f_x1 - loc_f_arc_center_x);
            SGLfloat loc_f_deltay = (par_f_y1 - loc_f_arc_center_y);

            SGLfloat loc_f_angle_end;
            SGLulong loc_ul_start_angle_pos;
            SGLulong loc_ul_end_angle_pos;

            loc_f_angle = mth_acos_degree(loc_f_deltax, loc_f_radius);
            loc_f_angle = mth_move_in_good_quadrant(loc_f_angle, loc_f_deltax, loc_f_deltay);

            loc_f_angle_end = loc_f_angle + (par_f_angle * loc_f_orientation);

            loc_f_angle_end = sgluModuloAngle(loc_f_angle_end);
            loc_f_angle = sgluModuloAngle(loc_f_angle);

            loc_ul_start_angle_pos = oglx_get_angle_position(loc_f_angle);
            loc_ul_end_angle_pos = oglx_get_angle_position(loc_f_angle_end);

            if (par_b_clockwise) {
                switch (loc_ul_start_angle_pos) {
                case OGLX_NE:
                {
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        if (loc_f_angle < loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    case OGLX_NW:
                        loc_b_east = SGL_TRUE;
                        loc_b_south = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    case OGLX_SW:
                        loc_b_east = SGL_TRUE;
                        loc_b_south = SGL_TRUE;
                        break;
                    default:
                        /* case OGLX_SE: */
                        loc_b_east = SGL_TRUE;
                        break;
                    }
                }
                    break;
                case OGLX_NW:
                {
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        loc_b_north = SGL_TRUE;
                        break;
                    case OGLX_NW:
                        if (loc_f_angle < loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    case OGLX_SW:
                        loc_b_east = SGL_TRUE;
                        loc_b_south = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        break;
                    default:
                        /* case OGLX_SE: */
                        loc_b_east = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        break;
                    }
                }
                    break;
                case OGLX_SW:
                {
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        loc_b_north = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    case OGLX_NW:
                        loc_b_west = SGL_TRUE;
                        break;
                    case OGLX_SW:
                        if (loc_f_angle < loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    default:
                        /* case OGLX_SE: */
                        loc_b_east = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        break;
                    }
                }
                    break;
                default:
                {
                    /* case OGLX_SE: */
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        loc_b_south = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    case OGLX_NW:
                        loc_b_south = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    case OGLX_SW:
                        loc_b_south = SGL_TRUE;
                        break;
                    default:
                        /* case OGLX_SE: */
                        if (loc_f_angle < loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    }
                }
                    break;
                }
            }
            else {
                switch (loc_ul_start_angle_pos) {
                case OGLX_NE:
                {
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        if (loc_f_angle > loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    case OGLX_NW:
                        loc_b_north = SGL_TRUE;
                        break;
                    case OGLX_SW:
                        loc_b_north = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    default:
                        /* case OGLX_SE: */
                        loc_b_north = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        loc_b_south = SGL_TRUE;
                        break;
                    }
                }
                    break;
                case OGLX_NW:
                {
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        loc_b_east = SGL_TRUE;
                        loc_b_south = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    case OGLX_NW:
                        if (loc_f_angle > loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    case OGLX_SW:
                        loc_b_west = SGL_TRUE;
                        break;
                    default:
                        /* case OGLX_SE: */
                        loc_b_south = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    }
                }
                    break;
                case OGLX_SW:
                {
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        loc_b_south = SGL_TRUE;
                        loc_b_east = SGL_TRUE;
                        break;
                    case OGLX_NW:
                        loc_b_south = SGL_TRUE;
                        loc_b_east = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        break;
                    case OGLX_SW:
                        if (loc_f_angle > loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    default:
                        /* case OGLX_SE: */
                        loc_b_south = SGL_TRUE;
                        break;
                    }
                }
                    break;
                default:
                {
                    /* case OGLX_SE: */
                    switch (loc_ul_end_angle_pos) {
                    case OGLX_NE:
                        loc_b_east = SGL_TRUE;
                        break;
                    case OGLX_NW:
                        loc_b_east = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        break;
                    case OGLX_SW:
                        loc_b_east = SGL_TRUE;
                        loc_b_north = SGL_TRUE;
                        loc_b_west = SGL_TRUE;
                        break;
                    default:
                        /* case OGLX_SE: */
                        if (loc_f_angle > loc_f_angle_end) {
                            loc_b_east = SGL_TRUE;
                            loc_b_south = SGL_TRUE;
                            loc_b_west = SGL_TRUE;
                            loc_b_north = SGL_TRUE;
                        }
                        else {
                            /* Nothing to do */
                        }
                        break;
                    }
                }
                    break;
                }
            }

            if (loc_b_north) {
                *par_pf_y2 = loc_f_arc_center_y + loc_f_radius;
            }
            else {
                *par_pf_y2 = loc_f_max_y;
            }

            if (loc_b_west) {
                *par_pf_x1 = loc_f_arc_center_x - loc_f_radius;
            }
            else {
                *par_pf_x1 = loc_f_min_x;
            }

            if (loc_b_south) {
                *par_pf_y1 = loc_f_arc_center_y - loc_f_radius;
            }
            else {
                *par_pf_y1 = loc_f_min_y;
            }

            if (loc_b_east) {
                *par_pf_x2 = loc_f_arc_center_x + loc_f_radius;
            }
            else {
                *par_pf_x2 = loc_f_max_x;
            }

        }
        else {
            *par_pf_x1 = par_f_x1;
            *par_pf_y1 = par_f_y1;
            *par_pf_x2 = par_f_x2;
            *par_pf_y2 = par_f_y2;
        }
    }
    return;
}


/* End of File ***************************************************************/
