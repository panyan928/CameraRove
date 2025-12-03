/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglPushMatrixAndTranslate.c
 DESCRIPTION       : sglPushMatrixAndTranslate command shall push the current modelview matrix and multiply the current modelview 
					matrix by a translation matrix
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
  NAME: sglPushMatrixAndTranslate
  DESCRIPTION:
    Function shall push the current modelview matrix and multiply the current modelview 
					matrix by a translation matrix
  SCOPE: global
  PARAMETERS:
    par_f_x -> X-coordinate of the translation vector
    par_f_y -> Y-coordinate of the translation vector
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglPushMatrixAndTranslate(SGLfloat par_f_x, SGLfloat par_f_y)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check stack is not full */
    if (glob_pr_sglStatemachine->ul_matrix_stack_index >= SGL_MAX_STACK) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLfloat *loc_pr_f1;
        SGLfloat *loc_pr_f2;
        sgl_stack *loc_pr_stack;
        SGLfloat loc_f_a1;
        SGLfloat loc_f_a2;
        SGLfloat loc_f_b1;
        SGLfloat loc_f_b2;

        loc_pr_stack = &glob_pr_sglStatemachine->tr_stack[glob_pr_sglStatemachine->ul_matrix_stack_index];

        /* Push the current modelview matrix on matrix stack */
        loc_pr_f1 = &loc_pr_stack->tf_modelview_matrix[0];
        loc_pr_f2 = &glob_pr_sglStatemachine->f_modelview_matrix[0];
        loc_pr_f1[12] = loc_pr_f2[12];
        loc_pr_f1[13] = loc_pr_f2[13];

        /* Increment stack index */
        glob_pr_sglStatemachine->ul_matrix_stack_index++;

        loc_pr_stack->f_depth = glob_pr_sglStatemachine->f_current_depth;

        /*
           For better readibility the model view matrix glob_pr_sglStatemachine->f_modelview_matrix
           is presented here as fi, i being the index of a value in the matrix.

           The new model view matrix is the matricial product between previous one and translation matrix:

           |f0          f4              f8              f12|.|1                 0               0               par_f_x|
           |f1          f5              f9              f13| |0                 1               0               par_f_y|
           |f2          f6              f10             f14| |0                 0               1                     0|
           |f3          f7              f11             f15| |0                 0               0                     1|

           =

           |f0          f4              f8              f0*par_f_x+f4*par_f_y+f12|
           |f1          f5              f9              f1*par_f_x+f5*par_f_y+f13|
           |f2          f6              f10                                                       f14|
           |f3          f7              f11                                                       f15|


         */
        loc_f_a1 = glob_pr_sglStatemachine->f_modelview_matrix[0];
        loc_f_b1 = glob_pr_sglStatemachine->f_modelview_matrix[1];

        loc_f_a2 = glob_pr_sglStatemachine->f_modelview_matrix[4];
        loc_f_b2 = glob_pr_sglStatemachine->f_modelview_matrix[5];

#ifdef ES2_DEV_ENV
        oglxPushMatrix();
#endif

        glob_pr_sglStatemachine->f_modelview_matrix[12] += ((loc_f_a1 * par_f_x) + (loc_f_a2 * par_f_y));

        glob_pr_sglStatemachine->f_modelview_matrix[13] += ((loc_f_b1 * par_f_x) + (loc_f_b2 * par_f_y));

#ifndef ES2_DEV_ENV
        glPushMatrix();
        glTranslatef(par_f_x, par_f_y, 0.0F);
#else
        oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif
    }
    else {
        oglxSetError(SGL_ERROR_SGL_PUSHMATRIXANDTRANSLATE, 0U);
    }

    return;
}

/* End of File ***************************************************************/
