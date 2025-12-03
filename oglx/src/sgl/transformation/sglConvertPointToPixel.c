/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglConvertPointToPixel.c
 DESCRIPTION       : sglConvertPointToPixel command shall convert the given coordinates to pixels coordinates
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
  NAME: sglConvertPointToPixel
  DESCRIPTION:
    Function shall convert the given coordinates to pixels coordinates
  SCOPE: global
  PARAMETERS:
    par_f_x -> X-coordinate in user unit
    par_f_y -> Y-coordinate in user unit
    par_pf_pixel_x -> X-coordinate in pixels
    par_pf_pixel_y -> Y-coordinate in pixels
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglConvertPointToPixel(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_pixel_x, SGLfloat * par_pf_pixel_y)
{
    /* Compute the coordinates with the modelview matrix */
    *par_pf_pixel_x =
        (((glob_pr_sglStatemachine->f_modelview_matrix[0] * par_f_x) +
          (glob_pr_sglStatemachine->f_modelview_matrix[4] * par_f_y) + (glob_pr_sglStatemachine->f_modelview_matrix[12]
                                                                        + 1.0F)) * glob_pr_sglStatemachine->f_viewport_width_div_2) +
        glob_pr_sglStatemachine->f_viewport_x1;

    *par_pf_pixel_y =
        (((glob_pr_sglStatemachine->f_modelview_matrix[1] * par_f_x) +
          (glob_pr_sglStatemachine->f_modelview_matrix[5] * par_f_y) + (glob_pr_sglStatemachine->f_modelview_matrix[13]
                                                                        + 1.0F)) * glob_pr_sglStatemachine->f_viewport_height_div_2) +
        glob_pr_sglStatemachine->f_viewport_y1;

    return;
}
