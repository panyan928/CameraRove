/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluArcSegment.c
 DESCRIPTION       : sgluArcSegment command shall define a segment
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

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_add_arc_segment
  DESCRIPTION:
    Function shall add an arc segment to current drawing.
  SCOPE: global
  PARAMETERS:
    par_f_x1 -> x coordinate of start point
    par_f_y1 -> y coordinate of start point
    par_f_x2 -> x coordinate of end point
    par_f_y2 -> y coordinate of end point
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_add_arc_segment(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLfloat loc_f_angle = glob_pr_sglStatemachine->f_arc_angle;
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
    SGLfloat loc_f_radius = SGLfloat_div(loc_f_norm, (2.0F * mth_cos_degree(loc_f_beta_angle)));
    SGLfloat loc_f_middle_x = ((loc_f_x2 + loc_f_x1) * 0.5F);
    SGLfloat loc_f_middle_y = ((loc_f_y2 + loc_f_y1) * 0.5F);

    SGLfloat loc_f_orientation;
    SGLfloat loc_f_arc_center_x;
    SGLfloat loc_f_arc_center_y;

    if (glob_pr_sglStatemachine->b_arc_clockwise) {
        loc_f_orientation = -1.0F;
    }
    else {
        loc_f_orientation = 1.0F;
    }

    if (loc_f_norm > SGL_MIN_F) {
        loc_f_arc_center_x =
            (loc_f_middle_x - (SGLfloat_div((loc_f_orientation * (loc_f_radius * mth_cos_degree(loc_f_angle * 0.5F))) * loc_f_vector_y, loc_f_norm)));

        loc_f_arc_center_y =
            (loc_f_middle_y + (SGLfloat_div((loc_f_orientation * (loc_f_radius * mth_cos_degree(loc_f_angle * 0.5F))) * loc_f_vector_x, loc_f_norm)));

        glob_pr_sglStatemachine->b_store_last_point = SGL_FALSE;
        /* Disable texture coordinates computation in sglVertex2f */
        sgluArcCircleLinePointsList2f(loc_f_arc_center_x, loc_f_arc_center_y, loc_f_x1, loc_f_y1, loc_f_radius, loc_f_radius,
                                      glob_pr_sglStatemachine->b_arc_clockwise, loc_f_angle);
        glob_pr_sglStatemachine->b_store_last_point = SGL_TRUE;
    }
    else {
        oglxVertex2f(par_f_x2, par_f_y2);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglu_arc_segment_detect_error
  DESCRIPTION:
    Function shall detect errors on sgluArcSegment.
  SCOPE: global
  PARAMETERS:
    par_b_clockwise -> Orientation of segment
    par_f_angle -> Angle of segment
  RETURN:
    SGLbool -> SGL_TRUE if an error is detected, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sglu_arc_segment_detect_error(SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    SGLbool loc_b_return = SGL_FALSE;
    SGLfloat loc_f_angle = sgluModuloAngle(par_f_angle);

    if ((par_b_clockwise != SGL_TRUE) && (par_b_clockwise != SGL_FALSE)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX) {
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
  NAME: sgluArcSegment
  DESCRIPTION:
    Function shall define an arc segment.
  SCOPE: global
  PARAMETERS:
    par_b_clockwise -> Orientation of segment
    par_f_angle -> Angle of segment
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgluArcSegment(SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    SGLbool loc_b_error_detected = sglu_arc_segment_detect_error(par_b_clockwise, par_f_angle);

    if (loc_b_error_detected) {
        oglxSetError(SGL_ERROR_SGLU_ARCSEGMENT, 0U);
    }
    else {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
        if (mth_f_abs(par_f_angle) > 2.0F) {
            glob_pr_sglStatemachine->b_arc_clockwise = par_b_clockwise;
            glob_pr_sglStatemachine->f_arc_angle = par_f_angle;
            glob_pr_sglStatemachine->b_arc_added = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
    }

    return;
}

/* End of File ***************************************************************/
