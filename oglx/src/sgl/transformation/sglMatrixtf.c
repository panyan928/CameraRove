/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglMatrixtf.c
 DESCRIPTION       : sglMatrixtf command shall multiply model view matrix with given matrix
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
  NAME: sglMatrixtf
  DESCRIPTION:
    Function shall multiply model view matrix with given matrix.
  SCOPE: global
  PARAMETERS:
    par_pf_matrix -> Matrix to apply to modelview matrix
  RETURN:
    void
---------------------------------------------------------------------+*/
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M8.3, 4, "Rule M8.3 is not checked here RTRT CR does not support the declaration of fixed size tables.")
#pragma attol crc_justify_all (Rule M16.7, 4, "Rule M16.7 is not applied here as a the const qualifier is used on the structure.")
#endif
void sglMatrixtf(const SGLfloat par_pf_matrix[3][3])
{
    SGLlong loc_l_i;
    SGLlong loc_l_j;
    SGLlong loc_l_k;
    SGLfloat loc_tf_transform[16];
    SGLfloat loc_tf_result[16];
    const SGLfloat *loc_tf_input_matrix = &(par_pf_matrix[0][0]);

    /* Copy the 3*3 matrix in a 4*4 one */
    loc_tf_transform[0] = loc_tf_input_matrix[0];
    loc_tf_transform[1] = loc_tf_input_matrix[1];
    loc_tf_transform[2] = 0.0F;
    loc_tf_transform[3] = loc_tf_input_matrix[2];

    loc_tf_transform[4] = loc_tf_input_matrix[3];
    loc_tf_transform[5] = loc_tf_input_matrix[4];
    loc_tf_transform[6] = 0.0F;
    loc_tf_transform[7] = loc_tf_input_matrix[5];

    loc_tf_transform[8] = 0.0F;
    loc_tf_transform[9] = 0.0F;
    loc_tf_transform[10] = 1.0F;
    loc_tf_transform[11] = 0.0F;

    loc_tf_transform[12] = loc_tf_input_matrix[6];
    loc_tf_transform[13] = loc_tf_input_matrix[7];
    loc_tf_transform[14] = 0.0F;
    loc_tf_transform[15] = loc_tf_input_matrix[8];

    /* Multiply the transformation matrix by the model view matrix */
    for (loc_l_i = 0; loc_l_i < 4L; loc_l_i++) {
        for (loc_l_j = 0; loc_l_j < 4L; loc_l_j++) {
            loc_tf_result[((loc_l_i * 4L) + loc_l_j)] = 0.0F;
            for (loc_l_k = 0; loc_l_k < 4L; loc_l_k++) {
                loc_tf_result[((loc_l_i * 4L) + loc_l_j)] +=
                    loc_tf_transform[((loc_l_i * 4L) + loc_l_k)] * glob_pr_sglStatemachine->f_modelview_matrix[((loc_l_k * 4L) + loc_l_j)];
            }
        }
    }

    /* Copy the computed matrix to the model view matrix */
    for (loc_l_i = 0; loc_l_i < 16L; loc_l_i++) {
        glob_pr_sglStatemachine->f_modelview_matrix[loc_l_i] = loc_tf_result[loc_l_i];
    }

#ifndef ES2_DEV_ENV
    glLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#else
    oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif

    return;
}
