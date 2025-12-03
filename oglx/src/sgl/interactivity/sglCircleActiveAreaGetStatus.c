/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCircleActiveAreaGetStatus.c
 DESCRIPTION       : sglCircleActiveAreaGetStatus command shall return the status 
					on the position of the mouse: SGL_TRUE if the mouse is inside 
					the circle area, SGL_FALSE if the mouse is outside the circle area
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
  NAME: sglCircleActiveAreaGetStatus
  DESCRIPTION:
    Function shall return the status on  the position of 
    the mouse: SGL_TRUE if the mouse is inside the circle area, 
    SGL_FALSE if the mouse is outside the circle area.
  SCOPE: global
  PARAMETERS:  
    par_f_mouse_x  -> x-coordinate for mouse
    par_f_mouse_y  -> y-coordinate for mouse
    par_f_origin_x -> x-coordinate for circle origin
    par_f_origin_y -> y-coordinate for circle origin
    par_f_radius   -> radius for circle            
  RETURN:
    SGLbyte -> SGL_TRUE: the mouse is in the circle
    SGL_FALSE: the mouse is out of the circle  
---------------------------------------------------------------------+*/
SGLbool sglCircleActiveAreaGetStatus(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_origin_x, SGLfloat par_f_origin_y, SGLfloat par_f_radius)
{
    SGLbyte loc_b_return = SGL_FALSE;
    SGLbyte loc_b_inside_scissors = SGL_TRUE;

    SGLfloat loc_f_origin_x_pixel;
    SGLfloat loc_f_origin_y_pixel;
    SGLfloat loc_f_mouse_x = 0.0F;
    SGLfloat loc_f_mouse_y = 0.0F;

    SGLfloat loc_f_radius_pixel_square_circle;
    SGLfloat loc_f_radius_pixel_square_position;
    SGLfloat loc_f_ratio;
    SGLbyte loc_b_float_is_equal;

    par_f_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    par_f_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    if (par_f_radius > 0.0F) {
        /* Convert the origin position in pixel coordinate system */
        loc_f_origin_x_pixel =
            (((glob_pr_sglStatemachine->f_modelview_matrix[0] * par_f_origin_x) + (glob_pr_sglStatemachine->f_modelview_matrix[4] * par_f_origin_y) +
              glob_pr_sglStatemachine->f_modelview_matrix[12] + 1.0F) * glob_pr_sglStatemachine->f_viewport_width_div_2) +
            glob_pr_sglStatemachine->f_viewport_x1;

        loc_f_origin_y_pixel =
            (((glob_pr_sglStatemachine->f_modelview_matrix[1] * par_f_origin_x) + (glob_pr_sglStatemachine->f_modelview_matrix[5] * par_f_origin_y) +
              glob_pr_sglStatemachine->f_modelview_matrix[13] + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2) +
            glob_pr_sglStatemachine->f_viewport_y1;

        /* Take into account the scale */
        loc_f_mouse_x = SGLfloat_div(par_f_mouse_x, glob_pr_sglStatemachine->f_scale_x);
        loc_f_mouse_y = SGLfloat_div(par_f_mouse_y, glob_pr_sglStatemachine->f_scale_y);
        loc_f_origin_x_pixel = SGLfloat_div(loc_f_origin_x_pixel, glob_pr_sglStatemachine->f_scale_x);
        loc_f_origin_y_pixel = SGLfloat_div(loc_f_origin_y_pixel, glob_pr_sglStatemachine->f_scale_y);

        loc_f_ratio = (glob_pr_sglStatemachine->f_ratio_scale_x + glob_pr_sglStatemachine->f_ratio_scale_y) * 0.5F;

        loc_b_float_is_equal = sgluFloatIsEqual(loc_f_ratio, 0.0F);
        if (loc_b_float_is_equal != SGL_TRUE) {
            /* Compute the square radius */
            loc_f_radius_pixel_square_circle = SGLfloat_div(par_f_radius, loc_f_ratio);
            loc_f_radius_pixel_square_circle = loc_f_radius_pixel_square_circle * loc_f_radius_pixel_square_circle;

            /* compute the square of the distance between mouse position and the center of the circle */
            loc_f_radius_pixel_square_position = ((loc_f_mouse_x - loc_f_origin_x_pixel) * (loc_f_mouse_x - loc_f_origin_x_pixel))
                + ((loc_f_mouse_y - loc_f_origin_y_pixel) * (loc_f_mouse_y - loc_f_origin_y_pixel));

            /* If the distance between  mouse position and center is less than the radius, the mouse is inside, else it is outside */
            if (loc_f_radius_pixel_square_position < loc_f_radius_pixel_square_circle) {
                loc_b_return = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Consider the mouse is in the circle */
            loc_b_return = SGL_TRUE;
        }
    }
    else {
        /* Nothing to do */
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
