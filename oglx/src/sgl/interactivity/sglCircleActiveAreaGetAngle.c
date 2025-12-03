/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCircleActiveAreaGetAngle.c
 DESCRIPTION       : sglCircleActiveAreaGetAngle command shall return the angle 
					value between the vertical set by the center of the circle 
					active area and the mouse position
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

#define SGL_HALF_PIXEL 0.5F


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglCircleActiveAreaGetAngle
  DESCRIPTION:
    Function shall return the angle value between the vertical 
    set by the center of the circle active area and the 
    mouse position.
  SCOPE: global
  PARAMETERS:  
    par_f_mouse_x  -> x-coordinate for mouse
    par_f_mouse_y  -> y-coordinate for mouse
    par_f_origin_x -> x-coordinate for circle origin
    par_f_origin_y -> y-coordinate for circle origin            
  RETURN:
    SGLfloat -> Angle between mouse position and horizontal of center, range: [0.0F, 360.0F[      
---------------------------------------------------------------------+*/
SGLfloat sglCircleActiveAreaGetAngle(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_origin_x, SGLfloat par_f_origin_y)
{
    SGLfloat loc_f_return;

    SGLfloat loc_f_origin_x_pixel;
    SGLfloat loc_f_origin_y_pixel;

    par_f_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    par_f_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    /* Convert the origin position in pixel coordinate system */
    loc_f_origin_x_pixel =
        (((glob_pr_sglStatemachine->f_modelview_matrix[0] * par_f_origin_x) + (glob_pr_sglStatemachine->f_modelview_matrix[4] * par_f_origin_y) +
          glob_pr_sglStatemachine->f_modelview_matrix[12] + 1.0F) * glob_pr_sglStatemachine->f_viewport_width_div_2) + glob_pr_sglStatemachine->f_viewport_x1;

    loc_f_origin_y_pixel =
        (((glob_pr_sglStatemachine->f_modelview_matrix[1] * par_f_origin_x) + (glob_pr_sglStatemachine->f_modelview_matrix[5] * par_f_origin_y) +
          glob_pr_sglStatemachine->f_modelview_matrix[13] + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2) + glob_pr_sglStatemachine->f_viewport_y1;

    if (((loc_f_origin_y_pixel - par_f_mouse_y) < SGL_HALF_PIXEL)
        && ((loc_f_origin_y_pixel - par_f_mouse_y) > -SGL_HALF_PIXEL)) {
        /* In case the mouse is at the same height as the center, return default value */
        if (par_f_mouse_x < loc_f_origin_x_pixel) {
            loc_f_return = 180.0F;
        }
        else {
            loc_f_return = 0.0F;
        }
    }
    else {
        /* The angle between mouse position and center is given by arc tangent */
        SGLfloat loc_f_tan = SGLfloat_div(par_f_mouse_x - loc_f_origin_x_pixel, par_f_mouse_y - loc_f_origin_y_pixel);
        loc_f_return = mth_atan_degree(loc_f_tan);

        if (par_f_mouse_y > loc_f_origin_y_pixel) {
            loc_f_return = 90.0F - loc_f_return;
        }
        else {
            loc_f_return = 270.0F - loc_f_return;
        }
    }

    return loc_f_return;
}

/* End of File ***************************************************************/
