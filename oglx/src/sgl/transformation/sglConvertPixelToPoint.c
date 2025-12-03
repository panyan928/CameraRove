/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglConvertPixelToPoint.c
 DESCRIPTION       : sglConvertPixelToPoint command shall convert the given pixel coordinates to user coordinates
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


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglConvertPixelToPoint
  DESCRIPTION:
    Function shall convert the given pixel coordinates to user coordinates
  SCOPE: global
  PARAMETERS:
    par_f_x -> X-coordinate in pixels
    par_f_y -> Y-coordinate in pixels
    par_pf_user_x -> X-coordinate in user units
    par_pf_user_y -> Y-coordinate in user units
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglConvertPixelToPoint(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_user_x, SGLfloat * par_pf_user_y)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_width_factor;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_height_factor;
    SGLfloat loc_f_f0 = glob_pr_sglStatemachine->f_modelview_matrix[0] * loc_f_ratio_x;
    SGLfloat loc_f_f1 = glob_pr_sglStatemachine->f_modelview_matrix[1];
    SGLfloat loc_f_f4 = glob_pr_sglStatemachine->f_modelview_matrix[4];
    SGLfloat loc_f_f5 = glob_pr_sglStatemachine->f_modelview_matrix[5] * loc_f_ratio_y;
    SGLfloat loc_f_f12 = glob_pr_sglStatemachine->f_modelview_matrix[12];
    SGLfloat loc_f_f13 = glob_pr_sglStatemachine->f_modelview_matrix[13];

    SGLfloat loc_f_f0_inv = SGLfloat_div(loc_f_f5, (loc_f_f5 * loc_f_f0) - (loc_f_f1 * loc_f_f4));
    SGLfloat loc_f_f1_inv = SGLfloat_div(-loc_f_f1, (loc_f_f5 * loc_f_f0) - (loc_f_f1 * loc_f_f4));
    SGLfloat loc_f_f4_inv = SGLfloat_div(-loc_f_f4, (loc_f_f5 * loc_f_f0) - (loc_f_f1 * loc_f_f4));
    SGLfloat loc_f_f5_inv = SGLfloat_div(loc_f_f0, (loc_f_f5 * loc_f_f0) - (loc_f_f1 * loc_f_f4));
    SGLfloat loc_f_f12_inv = SGLfloat_div((loc_f_f13 * loc_f_f4) - (loc_f_f5 * loc_f_f12), (loc_f_f5 * loc_f_f0) - (loc_f_f1 * loc_f_f4));
    SGLfloat loc_f_f13_inv = SGLfloat_div((-loc_f_f13 * loc_f_f0) + (loc_f_f1 * loc_f_f12), (loc_f_f5 * loc_f_f0) - (loc_f_f1 * loc_f_f4));
    SGLfloat loc_f_width_div_2 = glob_pr_sglStatemachine->f_viewport_width_div_2;
    SGLfloat loc_f_height_div_2 = glob_pr_sglStatemachine->f_viewport_height_div_2;

    par_f_x = par_f_x - loc_f_width_div_2;
    par_f_y = par_f_y - loc_f_height_div_2;

    *par_pf_user_x =
        (SGLfloat_div((loc_f_f0_inv * par_f_x) + (loc_f_f4_inv * par_f_y), loc_f_width_div_2) + loc_f_f12_inv -
         glob_pr_sglStatemachine->f_viewport_x1) * loc_f_ratio_x;
    *par_pf_user_y =
        (SGLfloat_div((loc_f_f1_inv * par_f_x) + (loc_f_f5_inv * par_f_y), loc_f_height_div_2) + loc_f_f13_inv -
         glob_pr_sglStatemachine->f_viewport_y1) * loc_f_ratio_y;

    return;
}
