/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxLoadMatrixf.c
 DESCRIPTION       : oglxLoadMatrixf command shall load given matrix.
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

#ifdef ES2_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxLoadMatrixf
  DESCRIPTION:
    Function shall load given matrix.
  SCOPE: global
  PARAMETERS:
    par_pf_matrix -> the matrix to load
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxLoadMatrixf(const SGLfloat * par_pf_matrix)
{
    glUniformMatrix4fv(glob_pr_sglStatemachine->ui_matrix_location, 1, GL_FALSE, par_pf_matrix);
    return;
}
#endif
