/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBeginScissor.c
 DESCRIPTION       : sglBeginScissor command shall start a new scissor
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
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_compute_scissor_intersection
  DESCRIPTION:
    Function shall compute the intersection between two scissor descriptions
  SCOPE: global
  PARAMETERS:
    par_p_old_scissor -> Scissor to take into acount
    par_p_new_scissor -> Scissor to update with intersection
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_compute_scissor_intersection(const scissor_definition * par_p_old_scissor, scissor_definition * par_p_new_scissor)
{
    SGLlong loc_i_new_x2 = par_p_new_scissor->i_x + par_p_new_scissor->s_width;
    SGLlong loc_i_new_y2 = par_p_new_scissor->i_y + par_p_new_scissor->s_height;

    SGLlong loc_i_old_x2 = par_p_old_scissor->i_x + par_p_old_scissor->s_width;
    SGLlong loc_i_old_y2 = par_p_old_scissor->i_y + par_p_old_scissor->s_height;

    if (par_p_new_scissor->i_x <= par_p_old_scissor->i_x) {
        par_p_new_scissor->i_x = par_p_old_scissor->i_x;
    }

    if (par_p_new_scissor->i_y <= par_p_old_scissor->i_y) {
        par_p_new_scissor->i_y = par_p_old_scissor->i_y;
    }

    if (loc_i_new_x2 < loc_i_old_x2) {
        if ((loc_i_new_x2 - par_p_new_scissor->i_x) > 0) {
            par_p_new_scissor->s_width = (loc_i_new_x2 - par_p_new_scissor->i_x);
        }
        else {
            par_p_new_scissor->s_width = 0;
        }
    }
    else {
        if ((loc_i_old_x2 - par_p_new_scissor->i_x) > 0) {
            par_p_new_scissor->s_width = (loc_i_old_x2 - par_p_new_scissor->i_x);
        }
        else {
            par_p_new_scissor->s_width = 0;
        }
    }

    if (loc_i_new_y2 < loc_i_old_y2) {
        if ((loc_i_new_y2 - par_p_new_scissor->i_y) > 0) {
            par_p_new_scissor->s_height = (loc_i_new_y2 - par_p_new_scissor->i_y);
        }
        else {
            par_p_new_scissor->s_height = 0;
        }
    }
    else {
        if ((loc_i_old_y2 - par_p_new_scissor->i_y) > 0) {
            par_p_new_scissor->s_height = (loc_i_old_y2 - par_p_new_scissor->i_y);
        }
        else {
            par_p_new_scissor->s_height = 0;
        }
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglBeginScissor
  DESCRIPTION:
    Function shall define a new scissor region.
  SCOPE: global
  PARAMETERS:
    par_f_width -> Scissor width
    par_f_height -> Scissor height
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglBeginScissor(SGLfloat par_f_width, SGLfloat par_f_height)
{
    if (glob_pr_sglStatemachine->ul_number_of_scissors < SGL_MAX_SCISSORS) {
        scissor_definition *loc_p_scissor = &glob_pr_sglStatemachine->ts_scissor_stack[glob_pr_sglStatemachine->ul_number_of_scissors];
        SGLfloat loc_f_x1;
        SGLfloat loc_f_y1;
        SGLfloat loc_f_x2;
        SGLfloat loc_f_y2;
        SGLfloat loc_f_tmp;
        SGLfloat loc_f_tmp2;

        /* Rotated scissors */
        loc_f_tmp = sgluModuloAngle(glob_pr_sglStatemachine->f_rotation_angle);
        if (sgluFloatIsEqual(loc_f_tmp, 90.0F)) {
            loc_f_tmp2 = par_f_width;
            par_f_width = -par_f_height;
            par_f_height = loc_f_tmp2;
        }
        else {
            if (sgluFloatIsEqual(loc_f_tmp, 270.0F)) {
                loc_f_tmp2 = par_f_width;
                par_f_width = par_f_height;
                par_f_height = -loc_f_tmp2;
            }
            else {
                if (sgluFloatIsEqual(loc_f_tmp, 180.0F)) {
                    par_f_width = -par_f_width;
                    par_f_height = -par_f_height;
                }
            }
        }

        /* Compute the coordinates with the modelview matrix */
        loc_f_x1 = ((glob_pr_sglStatemachine->f_modelview_matrix[12]
                     + 1.0F) * glob_pr_sglStatemachine->f_viewport_width_div_2 + glob_pr_sglStatemachine->f_viewport_x1);
        loc_f_y1 = ((glob_pr_sglStatemachine->f_modelview_matrix[13]
                     + 1.0F) * glob_pr_sglStatemachine->f_viewport_height_div_2 + glob_pr_sglStatemachine->f_viewport_y1);
        loc_f_x2 = loc_f_x1 + SGLfloat_div(par_f_width, glob_pr_sglStatemachine->f_ratio_x);
        loc_f_y2 = loc_f_y1 + SGLfloat_div(par_f_height, glob_pr_sglStatemachine->f_ratio_y);

        /* Sort the transformed coordinates */
        if (loc_f_x2 < loc_f_x1) {
            loc_f_tmp = loc_f_x1;
            loc_f_x1 = loc_f_x2;
            loc_f_x2 = loc_f_tmp;
        }
        if (loc_f_y2 < loc_f_y1) {
            loc_f_tmp = loc_f_y1;
            loc_f_y1 = loc_f_y2;
            loc_f_y2 = loc_f_tmp;
        }

        /* Store integer coordinates */
        loc_p_scissor->i_x = (SGLlong) loc_f_x1;
        loc_p_scissor->i_y = (SGLlong) loc_f_y1;
        loc_f_tmp = loc_f_x2 - loc_f_x1;
        loc_p_scissor->s_width = (SGLlong) loc_f_tmp;
        loc_f_tmp = loc_f_y2 - loc_f_y1;
        loc_p_scissor->s_height = (SGLlong) loc_f_tmp;

        if (glob_pr_sglStatemachine->ul_number_of_scissors == 0UL) {
            /* Check that the first scissor fit into the viewport */
            scissor_definition loc_s_viewport;
            loc_s_viewport.i_x = (SGLlong) glob_pr_sglStatemachine->f_viewport_x1;
            loc_s_viewport.i_y = (SGLlong) glob_pr_sglStatemachine->f_viewport_y1;
            loc_f_tmp = 2.0F * glob_pr_sglStatemachine->f_viewport_width_div_2;
            loc_s_viewport.s_width = (SGLlong) loc_f_tmp;
            loc_f_tmp = 2.0F * glob_pr_sglStatemachine->f_viewport_height_div_2;
            loc_s_viewport.s_height = (SGLlong) loc_f_tmp;

            sgl_compute_scissor_intersection(&loc_s_viewport, loc_p_scissor);
        }
        else {
            sgl_compute_scissor_intersection(&glob_pr_sglStatemachine->ts_scissor_stack[glob_pr_sglStatemachine->ul_number_of_scissors - 1U], loc_p_scissor);
        }

        glScissor(loc_p_scissor->i_x, loc_p_scissor->i_y, loc_p_scissor->s_width, loc_p_scissor->s_height);

        glob_pr_sglStatemachine->ul_number_of_scissors++;

        if (glob_pr_sglStatemachine->ul_number_of_scissors == 1UL) {
            oglxEnable(GL_SCISSOR_TEST);
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_BEGINSCISSOR, (SGLulong) SGL_MAX_SCISSORS);
    }

    return;
}
