/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxSetError.c
 DESCRIPTION       : Function used to set an error in the OGLX error table
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
  NAME: oglxSetError
  DESCRIPTION:
    Function shall set an error in the OGLX error table.
  SCOPE: global
  PARAMETERS:		
    par_ui_error_id -> error identifier
    par_ui_wrong_parameter -> incorrect parameter
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglxSetError(SGLulong par_ui_error_id, SGLulong par_ui_wrong_parameter)
{
    /* Store the given error in errors table if this one is not full */
    if (glob_pr_sglStatemachine->ul_error_counter < SGL_ERROR_MAX) {
        glob_pr_sglStatemachine->tul_error_table[glob_pr_sglStatemachine->ul_error_counter][0]
            = par_ui_error_id;

        glob_pr_sglStatemachine->tul_error_table[glob_pr_sglStatemachine->ul_error_counter][1]
            = par_ui_wrong_parameter;

        glob_pr_sglStatemachine->ul_error_counter++;
    }
    else {
        glob_pr_sglStatemachine->b_overflow_flag = SGL_TRUE;
    }
    return;
}

/* End of File ***************************************************************/
