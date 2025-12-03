/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexLineStipplei.c
 DESCRIPTION       : sglIndexLineStipplei command shall set the current linestipple 
					index
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
  NAME: sglIndexLineStipplei
  DESCRIPTION:
    Function shall set the current linestipple index.
  SCOPE: global
  PARAMETERS:
    par_l_index -> line stipple index, range: [0,glob_pr_sglStatemachine->ul_nb_line_stipple-1]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglIndexLineStipplei(SGLlong par_l_index)
{
    if ((glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX) && (par_l_index >= 0)) {
        /* Report an OGLX error if the input parameter has an incorrect value */
        if (((SGLulong) par_l_index) < glob_pr_sglStatemachine->ul_nb_line_stipple) {
            /* Nothing to do */
        }
        else {
            oglxSetError(SGL_ERROR_SGL_INDEXLINESTIPPLE, (SGLulong) par_l_index);
            /* set the index to the last index by default */
            par_l_index = (SGLlong) (glob_pr_sglStatemachine->ul_nb_line_stipple - 1U);
        }

        glob_pr_sglStatemachine->pr_stipple = (sgl_linestipple *) & (glob_pr_sglStatemachine->tr_linestipple[par_l_index]);

        if (!(glob_pr_sglStatemachine->pr_stipple->b_all_on)) {
            glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;
        }
        else {
            glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXLINESTIPPLE, (SGLulong) par_l_index);
    }

    return;
}

/* End of File ***************************************************************/
