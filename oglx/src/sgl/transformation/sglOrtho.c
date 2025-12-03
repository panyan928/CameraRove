/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglOrtho.c
 DESCRIPTION       : sglOrtho command shall replace the current modelview matrix 
					by an orthographic matrix
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

/******************************************************************************
 **                           Local file declarations
 *****************************************************************************/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglOrtho
  DESCRIPTION:
    Function shall replace the current modelview matrix by an orthographic matrix.
  SCOPE: global
  PARAMETERS:
    par_f_left -> Coordinate for the left vertical plane
    par_f_right -> Coordinate for the right vertical plane
    par_f_bottom -> Coordinate for the bottom horizontal plane
    par_f_top -> Coordinate for the top horizontal plane            
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglOrtho(SGLfloat par_f_left, SGLfloat par_f_right, SGLfloat par_f_bottom, SGLfloat par_f_top)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 3, "Rule M12.6.2 is not applied here as a macro is used")
#endif
        SGLfloat loc_f_right_left = mth_f_abs((SGLfloat) (par_f_right - par_f_left));
        SGLfloat loc_f_top_bottom = mth_f_abs((SGLfloat) (par_f_top - par_f_bottom));

        /* Check if matrix stack is empty */
        if (glob_pr_sglStatemachine->ul_matrix_stack_index != 0U) {
            loc_b_error_detected = 1U;
        }
        else {
            /* Nothing to do */
        }

        /* Check right is different from left */
        if (loc_f_right_left < SGL_MIN_F) {
            loc_b_error_detected = 1U;
        }
        else {
            /* Nothing to do */
        }

        /* Check bottom is different from top */
        if (loc_f_top_bottom < SGL_MIN_F) {
            loc_b_error_detected = 1U;
        }
        else {
            /* Nothing to do */
        }

        if (!loc_b_error_detected) {

            SGLfloat loc_f_right_ad_left;

            SGLfloat loc_f_top_ad_bottom;

            SGLfloat loc_f_a1;
            SGLfloat loc_f_a2;
            SGLfloat loc_f_a3;
            SGLfloat loc_f_a4;

            SGLfloat loc_f_b1;
            SGLfloat loc_f_b2;
            SGLfloat loc_f_b3;
            SGLfloat loc_f_b4;

            SGLfloat loc_f_c1;
            SGLfloat loc_f_c2;
            SGLfloat loc_f_c3;
            SGLfloat loc_f_c4;

            SGLfloat loc_f_d1;
            SGLfloat loc_f_d2;
            SGLfloat loc_f_d3;
            SGLfloat loc_f_d4;

            loc_f_right_left = (SGLfloat) (par_f_right - par_f_left);
            loc_f_top_bottom = (SGLfloat) (par_f_top - par_f_bottom);

            glob_pr_sglStatemachine->f_ortho_left = par_f_left;
            glob_pr_sglStatemachine->f_ortho_right = par_f_right;
            glob_pr_sglStatemachine->f_ortho_bottom = par_f_bottom;
            glob_pr_sglStatemachine->f_ortho_top = par_f_top;


            loc_f_right_ad_left = (SGLfloat) (par_f_right + par_f_left);
            loc_f_top_ad_bottom = (SGLfloat) (par_f_top + par_f_bottom);

            loc_f_a1 = SGLfloat_div(2.0F, loc_f_right_left);
            loc_f_a2 = 0.0F;
            loc_f_a3 = 0.0F;
            loc_f_a4 = SGLfloat_div(-loc_f_right_ad_left, loc_f_right_left);

            loc_f_b1 = 0.0F;
            loc_f_b2 = SGLfloat_div(2.0F, loc_f_top_bottom);
            loc_f_b3 = 0.0F;
            loc_f_b4 = SGLfloat_div(-loc_f_top_ad_bottom, loc_f_top_bottom);

            loc_f_c1 = 0.0F;
            loc_f_c2 = 0.0F;

            loc_f_c3 = 1.0F;
            loc_f_c4 = 0.0F;


            loc_f_d1 = 0.0F;
            loc_f_d2 = 0.0F;
            loc_f_d3 = 0.0F;
            loc_f_d4 = 1.0F;

            /*
             ** initialize the new Ortographic matrix
             **
             **  |2/(right-left)     0       0           t1|
             **  |                                         |
             **  |   0           2/(top-bot) 0           t2|
             **  |                                         |
             **  |   0               0           1                        0|
             **  |                                         |
             **  |   0               0       0           1 |
             **  |                                         |
             **  equal to  :
             **  | a1 a2 a3 a4 |
             **  |             |
             **  | b1 b2 b3 b4 |
             **  |             |
             **  | c1 c2 c3 c4 |
             **  |             |
             **  | d1 d2 d3 d4 |
             **  |             |
             **  where :
             **      t1 = -(right + left)/(right - left)
             **      t2 = -(top + bot)/(top - bot)
             **
             */

            glob_pr_sglStatemachine->tf_ortho_matrix[0] = loc_f_a1;
            glob_pr_sglStatemachine->tf_ortho_matrix[1] = loc_f_b1;
            glob_pr_sglStatemachine->tf_ortho_matrix[2] = loc_f_c1;
            glob_pr_sglStatemachine->tf_ortho_matrix[3] = loc_f_d1;
            glob_pr_sglStatemachine->tf_ortho_matrix[4] = loc_f_a2;
            glob_pr_sglStatemachine->tf_ortho_matrix[5] = loc_f_b2;
            glob_pr_sglStatemachine->tf_ortho_matrix[6] = loc_f_c2;
            glob_pr_sglStatemachine->tf_ortho_matrix[7] = loc_f_d2;
            glob_pr_sglStatemachine->tf_ortho_matrix[8] = loc_f_a3;
            glob_pr_sglStatemachine->tf_ortho_matrix[9] = loc_f_b3;
            glob_pr_sglStatemachine->tf_ortho_matrix[10] = loc_f_c3;
            glob_pr_sglStatemachine->tf_ortho_matrix[11] = loc_f_d3;
            glob_pr_sglStatemachine->tf_ortho_matrix[12] = loc_f_a4;
            glob_pr_sglStatemachine->tf_ortho_matrix[13] = loc_f_b4;
            glob_pr_sglStatemachine->tf_ortho_matrix[14] = loc_f_c4;
            glob_pr_sglStatemachine->tf_ortho_matrix[15] = loc_f_d4;

            sglLoadIdentity();

        }
        else {
            oglxSetError(SGL_ERROR_SGL_ORTHO, 0U);
        }
    }
    else {
        /* Raise error */
        oglxSetError(SGL_ERROR_SGL_ORTHO, 0U);
    }

    return;
}

/* End of File ***************************************************************/
