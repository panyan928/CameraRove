/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxPopMatrix.c
 DESCRIPTION       : oglxPopMatrix command shall pop last matrix on the stack.
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"


#ifdef ES2_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxPopMatrix
  DESCRIPTION:
    Function shall pop last matrix.
  SCOPE: global
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxPopMatrix(void)
{
    SGLulong loc_ul_index;
    SGLulong loc_ul_stack_index = glob_pr_sglStatemachine->ul_pushed_matrix_number;

    if (loc_ul_stack_index > 0UL) {
        for (loc_ul_index = 0; loc_ul_index < 16; loc_ul_index++) {
            glob_pr_sglStatemachine->f_modelview_matrix[loc_ul_index] = glob_pr_sglStatemachine->pf_pushed_matrix[loc_ul_index][loc_ul_stack_index - 1];
        }

        glUniformMatrix4fv(glob_pr_sglStatemachine->ui_matrix_location, 1, GL_FALSE, glob_pr_sglStatemachine->f_modelview_matrix);
        glob_pr_sglStatemachine->ul_pushed_matrix_number--;
    }
    else {
        /* Stack is empty */
    }
    return;
}
#endif
