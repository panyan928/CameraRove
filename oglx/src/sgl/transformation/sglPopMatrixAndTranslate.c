/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglPopMatrixAndTranslate.c
 DESCRIPTION       : sglPopMatrixAndTranslate command shall pop the current modelview matrix for a translation
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
  NAME: sglPopMatrixAndTranslate
  DESCRIPTION:
    Function shall pop the current modelview matrix for a translation.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglPopMatrixAndTranslate(void)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check stack is not empty */
    if (glob_pr_sglStatemachine->ul_matrix_stack_index == 0UL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLfloat *loc_pr_f1;
        SGLfloat *loc_pr_f2;
        sgl_stack *loc_pr_stack;

        glob_pr_sglStatemachine->ul_matrix_stack_index--;

        loc_pr_stack = &glob_pr_sglStatemachine->tr_stack[glob_pr_sglStatemachine->ul_matrix_stack_index];

        /* Restore last stored matrix */
        loc_pr_f1 = &glob_pr_sglStatemachine->f_modelview_matrix[0];
        loc_pr_f2 = &loc_pr_stack->tf_modelview_matrix[0];
        loc_pr_f1[12] = loc_pr_f2[12];
        loc_pr_f1[13] = loc_pr_f2[13];

        glob_pr_sglStatemachine->f_current_depth = loc_pr_stack->f_depth;
#ifndef ES2_DEV_ENV
        glPopMatrix();
#else
        oglxPopMatrix();
#endif
    }
    else {
        oglxSetError(SGL_ERROR_SGL_POPMATRIXANDTRANSLATE, 0U);
    }

    return;
}

/* End of File ***************************************************************/
