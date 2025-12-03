/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglRotatef.c
 DESCRIPTION       : sglRotatef command shall multiply the current modelview 
					matrix by a rotation
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
  NAME: sglRotatef
  DESCRIPTION:
    Function shall multiply the current modelview matrix by a rotation.
  SCOPE: global
  PARAMETERS:
    par_f_angle -> Angle of rotation, in degrees
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglRotatef(SGLfloat par_f_angle)
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
        SGLfloat loc_f_angle_cos;
        SGLfloat loc_f_angle_sin;
        SGLfloat loc_f_temp0;
        SGLfloat loc_f_temp1;
        SGLfloat loc_f_temp4;
        SGLfloat loc_f_temp5;

        par_f_angle = sgluModuloAngle(par_f_angle);

        /* Special case if angle is 90 degrees */
        if (sgluFloatIsEqual(par_f_angle, 90.0F)) {
            loc_f_angle_cos = 0.0F;
            loc_f_angle_sin = 1.0F;
        }
        else {
            loc_f_angle_cos = mth_cos_degree(par_f_angle);
            loc_f_angle_sin = mth_sin_degree(par_f_angle);
        }

        /*              
           For better readibility the model view matrix glob_pr_sglStatemachine->f_modelview_matrix
           is presented here as fi, i being the index of a value in the matrix.

           The new model view matrix is the matricial product between previous one and rotation matrix:

           |f0          f4              f8              f12|.|loc_f_angle_cos   -loc_f_angle_sin        0               0|
           |f1          f5              f9              f13| |loc_f_angle_sin   loc_f_angle_cos         0               0|
           |f2          f6              f10             f14| |0                                 0                                       1               0|
           |f3          f7              f11             f15| |0                                 0                                       0               1|

           =

           |f0*loc_f_angle_cos+f4*loc_f_angle_sin               f0*(-loc_f_angle_sin)+f4*loc_f_angle_cos                f8              f12|
           |f1*loc_f_angle_cos+f5*loc_f_angle_sin               f1*(-loc_f_angle_sin)+f5*loc_f_angle_cos                f9              f13|
           |f2                                                                                  f6                                                                                              f10             f14|
           |f3                                                                                  f7                                                                                              f11             f15|


         */


        loc_f_temp0 = glob_pr_sglStatemachine->f_modelview_matrix[0];
        loc_f_temp1 = glob_pr_sglStatemachine->f_modelview_matrix[1];
        loc_f_temp4 = glob_pr_sglStatemachine->f_modelview_matrix[4];
        loc_f_temp5 = glob_pr_sglStatemachine->f_modelview_matrix[5];

        glob_pr_sglStatemachine->f_modelview_matrix[0] = (loc_f_temp0 * loc_f_angle_cos) + (loc_f_temp4 * loc_f_angle_sin);

        glob_pr_sglStatemachine->f_modelview_matrix[4] = (loc_f_temp0 * (-loc_f_angle_sin)) + (loc_f_temp4 * loc_f_angle_cos);

        glob_pr_sglStatemachine->f_modelview_matrix[1] = (loc_f_temp1 * loc_f_angle_cos) + (loc_f_temp5 * loc_f_angle_sin);

        glob_pr_sglStatemachine->f_modelview_matrix[5] = (loc_f_temp1 * (-loc_f_angle_sin)) + (loc_f_temp5 * loc_f_angle_cos);

        glob_pr_sglStatemachine->f_rotation_angle = glob_pr_sglStatemachine->f_rotation_angle + par_f_angle;

#ifndef ES2_DEV_ENV
        glRotatef(par_f_angle, 0.0F, 0.0F, 1.0F);
#else
        oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif
    }
    else {
        /* Raise error */
        oglxSetError(SGL_ERROR_SGL_ROTATEF, 0U);
    }

    return;
}

/* End of File ***************************************************************/
