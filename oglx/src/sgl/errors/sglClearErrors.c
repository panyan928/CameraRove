/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglClearErrors.c
 DESCRIPTION       : sglClearErrors command shall clear the list of raised errors
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
  NAME: sglClearErrors
  DESCRIPTION:
    Function shall clear the list of raised errors.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglClearErrors(void)
{
    SGLulong loc_ul_index;

    /* Set to 0 each entry of the error table */
    for (loc_ul_index = 0UL; loc_ul_index < SGL_ERROR_MAX; loc_ul_index++) {
        glob_pr_sglStatemachine->tul_error_table[loc_ul_index][0] = 0UL;
        glob_pr_sglStatemachine->tul_error_table[loc_ul_index][1] = 0UL;
    }

    glob_pr_sglStatemachine->ul_error_counter = 0UL;
    glob_pr_sglStatemachine->b_overflow_flag = SGL_FALSE;

    return;
}

/* End of File ***************************************************************/
