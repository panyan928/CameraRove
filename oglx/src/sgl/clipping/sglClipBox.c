/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglClipBox.c
 DESCRIPTION       : sglClipBox command shall set a clipbox
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

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_clip_box_detect_error
  DESCRIPTION: 
	Function shall check parameters of sglClipBox.
  SCOPE: global
  PARAMETERS:          
    par_l_number -> Clipbox to define (SGL_CLIP_BOX1 or SGL_CLIP_BOX2)
    par_b_type -> Type of the clipbox (SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE)
    par_f_x_bottom_left -> X-coordinate of the lower left corner
    par_f_y_bottom_left -> Y-coordinate of the lower left corner
    par_f_x_top_right -> X-coordinate of the higher right corner
    par_f_y_top_right -> Y-coordinate of the higher right corner
  RETURN:
    SGLbyte -> Error detected (1U) or not (0U)
---------------------------------------------------------------------+*/
SGLbyte sgl_clip_box_detect_error(SGLlong par_l_number, SGLbyte par_b_type, SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left,
                                  SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check cb number is SGL_CLIP_BOX1 or SGL_CLIP_BOX2 */
    if ((par_l_number != (SGLlong) SGL_CLIP_BOX1) && (par_l_number != (SGLlong) SGL_CLIP_BOX2)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check type is SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE */
    if ((par_b_type != SGL_VISIBLE_INSIDE) && (par_b_type != SGL_VISIBLE_OUTSIDE)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check the function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check par_f_x_top_right is different from par_f_x_bottom_left */
    if (((par_f_x_top_right - par_f_x_bottom_left) <= SGL_MIN_F) && ((par_f_x_top_right - par_f_x_bottom_left) >= -SGL_MIN_F)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check par_f_y_top_right is different from par_f_y_bottom_left */
    if (((par_f_y_top_right - par_f_y_bottom_left) <= SGL_MIN_F) && ((par_f_y_top_right - par_f_y_bottom_left) >= -SGL_MIN_F)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglClipBox
  DESCRIPTION: 
	Function shall set a clipbox.
  SCOPE: global
  PARAMETERS:          
    par_l_number -> Clipbox to define (SGL_CLIP_BOX1 or SGL_CLIP_BOX2)
    par_b_type -> Type of the clipbox (SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE)
    par_f_x_bottom_left -> X-coordinate of the lower left corner
    par_f_y_bottom_left -> Y-coordinate of the lower left corner
    par_f_x_top_right -> X-coordinate of the higher right corner
    par_f_y_top_right -> Y-coordinate of the higher right corner
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglClipBox(SGLlong par_l_number, SGLbyte par_b_type, SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right,
                SGLfloat par_f_y_top_right)
{

    SGLbyte loc_b_error_detected =
        sgl_clip_box_detect_error(par_l_number, par_b_type, par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_top_right);

    if (!loc_b_error_detected) {
        SGLulong loc_ul_clip_box_id;
        SGLbyte loc_b_visible_outside_state;

        /* Select the clip box id */
        if (par_l_number == (SGLlong) SGL_CLIP_BOX1) {
            loc_ul_clip_box_id = 0UL;
        }
        else {
            loc_ul_clip_box_id = 1UL;
        }

        if (par_b_type == SGL_VISIBLE_INSIDE) {
            loc_b_visible_outside_state = SGL_FALSE;
        }
        else {
            loc_b_visible_outside_state = SGL_TRUE;
        }

        /* Can not modify enabled clipbox */
        if (!(glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].b_state)) {
            glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].f_bottom_left_x = par_f_x_bottom_left;
            glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].f_bottom_left_y = par_f_y_bottom_left;
            glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].f_top_right_x = par_f_x_top_right;
            glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].f_top_right_y = par_f_y_top_right;
            glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].b_visible_outside = loc_b_visible_outside_state;
            glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].b_define = SGL_TRUE;

            sgl_stencil_buffer_ClipBox_description(loc_ul_clip_box_id);
            glob_pr_sglStatemachine->b_stencil_update = SGL_TRUE;

            /* If the clipbox is set as visible inside, */
            /*  get current matrix to compute the union of the clipboxes */
            /*  and to set the modelview matrix if the scissor is used */
            if (!(glob_pr_sglStatemachine->tr_clipbox[loc_ul_clip_box_id].b_visible_outside)) {

                /* Store the current modelview matrix */
                SGLulong loc_ul_i;
                SGLfloat *loc_pf_modelview_matrix_storage;

                /* Get storage address */
                if (par_l_number == (SGLlong) SGL_CLIP_BOX1) {
                    loc_pf_modelview_matrix_storage = glob_pr_sglStatemachine->tr_clipbox[0].pf_modelView;

                    /* Get current matrix */
                    for (loc_ul_i = 0UL; loc_ul_i < 16UL; loc_ul_i++) {
                        loc_pf_modelview_matrix_storage[loc_ul_i] = glob_pr_sglStatemachine->f_modelview_matrix[loc_ul_i];
                    }
                }
                else {          /* par_b_clip_object == SGL_CLIP_BOX2) */

                    loc_pf_modelview_matrix_storage = glob_pr_sglStatemachine->tr_clipbox[1].pf_modelView;

                    /* Get current matrix */
                    for (loc_ul_i = 0UL; loc_ul_i < 16UL; loc_ul_i++) {
                        loc_pf_modelview_matrix_storage[loc_ul_i] = glob_pr_sglStatemachine->f_modelview_matrix[loc_ul_i];
                    }
                }
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CLIPBOX, (SGLulong) par_l_number);
    }

    return;
}

/* End of File ***************************************************************/
