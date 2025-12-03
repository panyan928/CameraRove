/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexLineWidthi.c
 DESCRIPTION       : sglIndexLineWidthi command shall set the current linewidth 
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

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglIndexLineWidthi
  DESCRIPTION:
    Function shall set the current linewidth index.
  SCOPE: global
  PARAMETERS:
    par_l_index -> linewidth index, range: [0,glob_pr_sglStatemachine->ul_nb_line_width-1]
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglIndexLineWidthi(SGLlong par_l_index)
{
    if ((glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX) && (par_l_index >= 0)) {
        /* Report an error and set default value if input parameter is invalid */
        if (par_l_index < (SGLlong) glob_pr_sglStatemachine->ul_nb_line_width) {
            /* Nothing to do */
        }
        else {
            oglxSetError(SGL_ERROR_SGL_INDEXLINEWIDTHI, (SGLulong) par_l_index);
            par_l_index = glob_pr_sglStatemachine->ul_nb_line_width - 1U;
        }

        /* Store index line width in OGLX state machine */
        glob_pr_sglStatemachine->l_linewidth_index = par_l_index;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXLINEWIDTHI, (SGLulong) par_l_index);
    }

    return;
}

/* End of File ***************************************************************/
