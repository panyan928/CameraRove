/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexFontLineWidthi.c
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
  NAME: sglIndexFontLineWidthi
  DESCRIPTION:
    Function shall set the current font linewidth index.
  SCOPE: global
  PARAMETERS:
    par_l_index -> linewidth index, range: [0,7]
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglIndexFontLineWidthi(SGLlong par_l_index)
{
    if ((glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX) && (par_l_index >= 0)) {
        /* Report an error and set default value if input parameter is invalid */
        if (par_l_index < (SGLlong) FONT_LINE_WIDTH_SIZE) {
            /* Nothing to do */
        }
        else {
            oglxSetError(SGL_ERROR_SGL_INDEXFONTLINEWIDTHI, (SGLulong) par_l_index);
            par_l_index = FONT_LINE_WIDTH_SIZE - 1U;
        }

        /* Set index for Bitmap fonts */
        if (par_l_index < (SGLlong) 1) {
            glob_pr_sglStatemachine->b_current_font_linewidth = 0U;
        }
        else {
            glob_pr_sglStatemachine->b_current_font_linewidth = (SGLbyte) (par_l_index - 1);
        }

        /* Set index for Stroke fonts */
        glob_pr_sglStatemachine->l_current_font_linewidth_index = par_l_index;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXFONTLINEWIDTHI, (SGLulong) par_l_index);
    }

    return;
}

/* End of File ***************************************************************/
