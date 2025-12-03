/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglRectangleActiveAreaGetHeightRate.c
 DESCRIPTION       : sglRectangleActiveAreaGetHeightRate command shall return 
					the ratio for the distance between the mouse y-coordinate 
					with the y-coordinate of the low left corner of the rectangle 
					active area with the height of the rectangle active area
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
  NAME: sglRectangleActiveAreaGetHeightRate
  DESCRIPTION:
    Function shall return the ratio for the distance between the mouse 
    y-coordinate with the y-coordinate of the low left corner of the 
    rectangle active area with the height of the rectangle active area.
  SCOPE: global
  PARAMETERS:
    par_f_mouse_x -> x-coordinate for mouse
    par_f_mouse_y -> y-coordinate for mouse
    par_f_x1 -> x-coordinate for first point
    par_f_y1 -> y-coordinate for first point
    par_f_x2 -> x-coordinate for second point
    par_f_y2 -> y-coordinate for second point            
  RETURN:
    SGLfloat -> Vertical rate between mouse position and lower point 
    of rectangle for interactivity, range: [0.0F,1.0F]  
---------------------------------------------------------------------+*/
SGLfloat sglRectangleActiveAreaGetHeightRate(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2,
                                             SGLfloat par_f_y2)
{
    SGLfloat loc_f_return;

    par_f_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    par_f_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    if ((((par_f_x2 - par_f_x1) > -SGL_MIN_F) && ((par_f_x2 - par_f_x1) < SGL_MIN_F))
        || (((par_f_y2 - par_f_y1) > -SGL_MIN_F) && ((par_f_y2 - par_f_y1) < SGL_MIN_F))) {
        loc_f_return = 0.0F;
    }
    else {
        SGLfloat loc_f_y_1_pixel;
        SGLfloat loc_f_y_2_pixel;
        SGLfloat loc_f_min_y;
        SGLfloat loc_f_max_y;

        /* When there is no rotation, compute only y in pixels for performances */
        if (sgluFloatIsEqual(glob_pr_sglStatemachine->f_rotation_angle, 0.0F)) {

            /* Convert the origin position in pixel coordinate system */
            loc_f_y_1_pixel =
                (((glob_pr_sglStatemachine->f_modelview_matrix[1] * par_f_x1) + (glob_pr_sglStatemachine->f_modelview_matrix[5] * par_f_y1) +
                  glob_pr_sglStatemachine->f_modelview_matrix[13] + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2) +
                glob_pr_sglStatemachine->f_viewport_y1;

            loc_f_y_2_pixel =
                (((glob_pr_sglStatemachine->f_modelview_matrix[1] * par_f_x2) + (glob_pr_sglStatemachine->f_modelview_matrix[5] * par_f_y2) +
                  glob_pr_sglStatemachine->f_modelview_matrix[13] + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2) +
                glob_pr_sglStatemachine->f_viewport_y1;


            /* Compute upper and lower rectangle position */
            if (loc_f_y_1_pixel < loc_f_y_2_pixel) {
                loc_f_min_y = loc_f_y_1_pixel;
                loc_f_max_y = loc_f_y_2_pixel;
            }
            else {
                loc_f_min_y = loc_f_y_2_pixel;
                loc_f_max_y = loc_f_y_1_pixel;
            }
            /* If rectangle is large enough, compute the rate between mouse position and rectangle height */
            loc_f_return = SGLfloat_div(par_f_mouse_y - loc_f_min_y, loc_f_max_y - loc_f_min_y);
        }
        else {
            SGLfloat loc_f_x_mouse_proj, loc_f_y_mouse_proj;
            SGLfloat loc_f_x_1_pixel;
            SGLfloat loc_f_x_2_pixel;
            SGLfloat loc_f_k;
            SGLfloat loc_f_norm1;
            SGLfloat loc_f_norm2;
            SGLfloat loc_f_dot_product;
            SGLfloat loc_f_sign;

            sglConvertPointToPixel(par_f_x2, par_f_y1, &loc_f_x_1_pixel, &loc_f_y_1_pixel);
            sglConvertPointToPixel(par_f_x2, par_f_y2, &loc_f_x_2_pixel, &loc_f_y_2_pixel);

            /* Compute the projection of the mouse on a vertical segment of the rectangle */
            loc_f_k =
                SGLfloat_div(((loc_f_y_2_pixel - loc_f_y_1_pixel) * (par_f_mouse_x - loc_f_x_1_pixel)) -
                             ((loc_f_x_2_pixel - loc_f_x_1_pixel) * (par_f_mouse_y - loc_f_y_1_pixel)),
                             ((loc_f_y_2_pixel - loc_f_y_1_pixel) * (loc_f_y_2_pixel - loc_f_y_1_pixel)) +
                             ((loc_f_x_2_pixel - loc_f_x_1_pixel) * (loc_f_x_2_pixel - loc_f_x_1_pixel)));

            loc_f_x_mouse_proj = par_f_mouse_x - loc_f_k * (loc_f_y_2_pixel - loc_f_y_1_pixel);
            loc_f_y_mouse_proj = par_f_mouse_y + loc_f_k * (loc_f_x_2_pixel - loc_f_x_1_pixel);

            loc_f_norm1 =
                mth_sqrtf((loc_f_x_mouse_proj - loc_f_x_1_pixel) * (loc_f_x_mouse_proj - loc_f_x_1_pixel) +
                          (loc_f_y_mouse_proj - loc_f_y_1_pixel) * (loc_f_y_mouse_proj - loc_f_y_1_pixel));
            loc_f_norm2 =
                mth_sqrtf((loc_f_x_1_pixel - loc_f_x_2_pixel) * (loc_f_x_1_pixel - loc_f_x_2_pixel) +
                          (loc_f_y_1_pixel - loc_f_y_2_pixel) * (loc_f_y_1_pixel - loc_f_y_2_pixel));

            loc_f_dot_product =
                ((loc_f_x_mouse_proj - loc_f_x_1_pixel) * (loc_f_x_2_pixel - loc_f_x_1_pixel)) +
                ((loc_f_y_mouse_proj - loc_f_y_1_pixel) * (loc_f_y_2_pixel - loc_f_y_1_pixel));
            loc_f_sign = loc_f_dot_product > 0.0F ? 1.0F : -1.0F;

            loc_f_return = loc_f_sign * SGLfloat_div(loc_f_norm1, loc_f_norm2);
        }

    }

    return loc_f_return;
}

/* End of File ***************************************************************/
