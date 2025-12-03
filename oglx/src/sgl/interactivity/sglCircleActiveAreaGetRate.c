/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCircleActiveAreaGetRate.c
 DESCRIPTION       : sglCircleActiveAreaGetRate command shall return the ratio 
					of the distance of the mouse coordinates and the radius of 
					the circle active area radius.
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


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglCircleActiveAreaGetRate
  DESCRIPTION:
    Function shall return the ratio of  the distance of 
    the mouse coordinates and the radius of the circle 
    active area radius.
  SCOPE: global
  PARAMETERS:  
    par_f_mouse_x  -> x-coordinate for mouse
    par_f_mouse_y  -> y-coordinate for mouse
    par_f_origin_x -> x-coordinate for circle origin
    par_f_origin_y -> y-coordinate for circle origin
    par_f_radius   -> radius for circle            
  RETURN:
    SGLfloat -> Rate between mouse position relativeto center and radius, range: [0.0F,1.0F]
---------------------------------------------------------------------+*/
SGLfloat sglCircleActiveAreaGetRate(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_origin_x, SGLfloat par_f_origin_y, SGLfloat par_f_radius)
{
    SGLfloat loc_f_return;

    SGLfloat loc_f_origin_x_pixel;
    SGLfloat loc_f_origin_y_pixel;
    SGLfloat loc_f_radius_pixel_square_circle;
    SGLfloat loc_f_radius_pixel_square_position;
    SGLfloat loc_f_ratio;
    SGLbyte loc_b_float_is_equal;

    par_f_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    par_f_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    /* Check if radius is positive */
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
        par_f_mouse_x = SGLfloat_div(par_f_mouse_x, glob_pr_sglStatemachine->f_scale_x);
        par_f_mouse_y = SGLfloat_div(par_f_mouse_y, glob_pr_sglStatemachine->f_scale_y);
        loc_f_origin_x_pixel = SGLfloat_div(loc_f_origin_x_pixel, glob_pr_sglStatemachine->f_scale_x);
        loc_f_origin_y_pixel = SGLfloat_div(loc_f_origin_y_pixel, glob_pr_sglStatemachine->f_scale_y);

        loc_f_ratio = (glob_pr_sglStatemachine->f_ratio_scale_x + glob_pr_sglStatemachine->f_ratio_scale_y) * 0.5F;

        loc_b_float_is_equal = sgluFloatIsEqual(loc_f_ratio, 0.0F);
        if (loc_b_float_is_equal != SGL_TRUE) {
            /* Compute the square radius */
            loc_f_radius_pixel_square_circle = SGLfloat_div(par_f_radius, loc_f_ratio);
            loc_f_radius_pixel_square_circle = loc_f_radius_pixel_square_circle * loc_f_radius_pixel_square_circle;

            /* Compute the square distance between center and mouse position */
            loc_f_radius_pixel_square_position = ((par_f_mouse_x - loc_f_origin_x_pixel) * (par_f_mouse_x - loc_f_origin_x_pixel))
                + ((par_f_mouse_y - loc_f_origin_y_pixel) * (par_f_mouse_y - loc_f_origin_y_pixel));

            loc_f_return = mth_sqrtf(SGLfloat_div(loc_f_radius_pixel_square_position, loc_f_radius_pixel_square_circle));
        }
        else {
            loc_f_return = 0.0F;
        }
    }
    else {
        loc_f_return = 0.0F;
    }

    return loc_f_return;
}

/* End of File ***************************************************************/
