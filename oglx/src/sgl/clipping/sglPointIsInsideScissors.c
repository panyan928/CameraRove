/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglPointIsInsideScissors.c
 DESCRIPTION       : sglPointIsInsideScissors command shall check if given point is inside active scissors
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
  NAME: sgl_pixel_point_is_inside_scissors
  DESCRIPTION:
    Function shall shall check if given point is inside active scissors.
  SCOPE: global
  PARAMETERS:
    par_f_x -> X coordinate
    par_f_y -> Y coordinate
  RETURN:
    SGLbool -> SGL_TRUE if the point is inside scissors, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_pixel_point_is_inside_scissors(SGLfloat par_f_x, SGLfloat par_f_y)
{
    SGLbool loc_b_return = SGL_TRUE;
    SGLlong loc_l_x = mth_f_to_l(par_f_x);
    SGLlong loc_l_y = mth_f_to_l(par_f_y);

    if (par_f_x < 0.0F) {
        loc_l_x = -loc_l_x;
    }
    else {
        /* Nothing to do */
    }

    if (par_f_y < 0.0F) {
        loc_l_y = -loc_l_y;
    }
    else {
        /* Nothing to do */
    }

    if (glob_pr_sglStatemachine->ul_number_of_scissors > 0) {
        SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_number_of_scissors - 1U;
        scissor_definition loc_s_scissor = glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index];

        if ((loc_l_x >= loc_s_scissor.i_x) && (loc_l_x <= (loc_s_scissor.i_x + loc_s_scissor.s_width))
            && (loc_l_y >= loc_s_scissor.i_y) && (loc_l_y <= (loc_s_scissor.i_y + loc_s_scissor.s_height))) {
            loc_b_return = SGL_TRUE;
        }
        else {
            loc_b_return = SGL_FALSE;
        }
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglPointIsInsideScissors
  DESCRIPTION:
    Function shall shall check if given point is inside active scissors.
  SCOPE: global
  PARAMETERS:
    par_f_x -> X coordinate
    par_f_y -> Y coordinate
  RETURN:
    SGLbool -> SGL_TRUE if the point is inside scissors, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sglPointIsInsideScissors(SGLfloat par_f_x, SGLfloat par_f_y)
{
    SGLfloat loc_f_x_pixel = 0.0F;
    SGLfloat loc_f_y_pixel = 0.0F;

    sglConvertPointToPixel(par_f_x, par_f_y, &loc_f_x_pixel, &loc_f_y_pixel);

    /* The comparison here is performed in pixels */
    return sgl_pixel_point_is_inside_scissors(loc_f_x_pixel, loc_f_y_pixel);
}
