/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglRectangleIntersectsScissors.c
 DESCRIPTION       : sglRectangleIntersectsScissors command shall check if given rectangle is inside active scissors
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
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_pixel_rectangle_is_inside_scissors
  DESCRIPTION:
    Function shall check if given point is inside active scissors.
  SCOPE: global
  PARAMETERS:
    par_f_x1, par_f_y1 -> coordinates of the first point
    par_f_x2, par_f_y2 -> coordinates of the second point
  RETURN:
    SGLbool -> SGL_TRUE if the rectangle is inside scissors, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_pixel_rectangle_is_inside_scissors(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLbool loc_b_return = SGL_FALSE;
    SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_number_of_scissors - 1U;
    SGLlong loc_l_x1 = mth_f_to_l(par_f_x1);
    SGLlong loc_l_y1 = mth_f_to_l(par_f_y1);
    SGLlong loc_l_x2 = mth_f_to_l(par_f_x2);
    SGLlong loc_l_y2 = mth_f_to_l(par_f_y2);

    /* Here we are sure there is at least one scissor as it is already checked by the calls to sglPointIsInsideScissors in sglRectangleIntersectsScissors */
    scissor_definition loc_s_scissor = glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index];

    if (par_f_x1 < 0.0F) {
        loc_l_x1 = -loc_l_x1;
    }
    else {
        /* Nothing to do */
    }

    if (par_f_y1 < 0.0F) {
        loc_l_y1 = -loc_l_y1;
    }
    else {
        /* Nothing to do */
    }

    if (par_f_x2 < 0.0F) {
        loc_l_x2 = -loc_l_x2;
    }
    else {
        /* Nothing to do */
    }

    if (par_f_y2 < 0.0F) {
        loc_l_y2 = -loc_l_y2;
    }
    else {
        /* Nothing to do */
    }



    if ((loc_l_x1 <= loc_s_scissor.i_x) && (loc_l_x2 >= (loc_s_scissor.i_x + loc_s_scissor.s_width))
        && (loc_l_y1 <= loc_s_scissor.i_y) && (loc_l_y2 >= (loc_s_scissor.i_y + loc_s_scissor.s_height))) {
        loc_b_return = SGL_TRUE;
    }
    else {
        if ((loc_l_x1 <= loc_s_scissor.i_x) && (loc_l_x2 >= (loc_s_scissor.i_x + loc_s_scissor.s_width))
            && (loc_l_y1 >= loc_s_scissor.i_y) && (loc_l_y1 <= (loc_s_scissor.i_y + loc_s_scissor.s_height))) {
            loc_b_return = SGL_TRUE;
        }
        else {
            if ((loc_l_x1 <= loc_s_scissor.i_x) && (loc_l_x2 >= (loc_s_scissor.i_x + loc_s_scissor.s_width))
                && (loc_l_y2 >= loc_s_scissor.i_y) && (loc_l_y2 <= (loc_s_scissor.i_y + loc_s_scissor.s_height))) {
                loc_b_return = SGL_TRUE;
            }
            else {
                if ((loc_l_y1 <= loc_s_scissor.i_y) && (loc_l_y2 >= (loc_s_scissor.i_y + loc_s_scissor.s_height))
                    && (loc_l_x1 >= loc_s_scissor.i_x) && (loc_l_x1 <= (loc_s_scissor.i_x + loc_s_scissor.s_width))) {
                    loc_b_return = SGL_TRUE;
                }
                else {
                    if ((loc_l_y1 <= loc_s_scissor.i_y) && (loc_l_y2 >= (loc_s_scissor.i_y + loc_s_scissor.s_height))
                        && (loc_l_x2 >= loc_s_scissor.i_x) && (loc_l_x2 <= (loc_s_scissor.i_x + loc_s_scissor.s_width))) {
                        loc_b_return = SGL_TRUE;
                    }
                    else {
                        /* Nothing to do */
                    }
                }
            }
        }
    }

    return loc_b_return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglRectangleIntersectsScissors
  DESCRIPTION:
    Function shall check if given rectangle is inside active scissors.
  SCOPE: global
  PARAMETERS:
    par_f_x1, par_f_y1 -> coordinates of the first point
    par_f_x2, par_f_y2 -> coordinates of the second point
  RETURN:
    SGLbool -> SGL_TRUE if the rectangle is inside scissors, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sglRectangleIntersectsScissors(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLbool loc_b_return = SGL_FALSE;

    SGLfloat loc_f_x1_pixel = 0.0F;
    SGLfloat loc_f_y1_pixel = 0.0F;
    SGLfloat loc_f_x2_pixel = 0.0F;
    SGLfloat loc_f_y2_pixel = 0.0F;

    SGLfloat loc_f_x1 = 0.0F;
    SGLfloat loc_f_y1 = 0.0F;
    SGLfloat loc_f_x2 = 0.0F;
    SGLfloat loc_f_y2 = 0.0F;

    /* Sort the input values */
    if (par_f_x1 < par_f_x2) {
        loc_f_x1 = par_f_x1;
        loc_f_x2 = par_f_x2;
    }
    else {
        loc_f_x1 = par_f_x2;
        loc_f_x2 = par_f_x1;
    }

    if (par_f_y1 < par_f_y2) {
        loc_f_y1 = par_f_y1;
        loc_f_y2 = par_f_y2;
    }
    else {
        loc_f_y1 = par_f_y2;
        loc_f_y2 = par_f_y1;
    }

    sglConvertPointToPixel(loc_f_x1, loc_f_y1, &loc_f_x1_pixel, &loc_f_y1_pixel);
    sglConvertPointToPixel(loc_f_x2, loc_f_y2, &loc_f_x2_pixel, &loc_f_y2_pixel);

    if (sglPointIsInsideScissors(loc_f_x1, loc_f_y1)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        if (sglPointIsInsideScissors(loc_f_x1, loc_f_y2)) {
            loc_b_return = SGL_TRUE;
        }
        else {
            if (sglPointIsInsideScissors(loc_f_x2, loc_f_y2)) {
                loc_b_return = SGL_TRUE;
            }
            else {
                if (sglPointIsInsideScissors(loc_f_x2, loc_f_y1)) {
                    loc_b_return = SGL_TRUE;
                }
                else {
                    /* Nothing to do */
                }
            }
        }
    }

    if (!loc_b_return) {
        loc_b_return = sgl_pixel_rectangle_is_inside_scissors(loc_f_x1_pixel, loc_f_y1_pixel, loc_f_x2_pixel, loc_f_y2_pixel);
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}
