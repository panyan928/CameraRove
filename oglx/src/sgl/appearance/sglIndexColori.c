/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexColori.c
 DESCRIPTION       : sglIndexColori command shall set the current color with 
					color set at the specified index
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
  NAME: sglIndexColori
  DESCRIPTION:
    Function shall set the current color with color set 
    at the specified index.
  SCOPE: global
  PARAMETERS:
    par_l_index -> color index, range: [0,glob_pr_sglStatemachine->ul_nb_colors-1]
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglIndexColori(SGLlong par_l_index)
{

    SGLbyte loc_b_error_detected = 0U;

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if ((par_l_index < 0L) || (par_l_index >= (SGLlong) glob_pr_sglStatemachine->ul_nb_colors)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check if input parameter has a valid value */
    if (!loc_b_error_detected) {
        /* Nothing to do */
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXCOLORI, (SGLulong) par_l_index);
        par_l_index = (SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1U);
    }

    glob_pr_sglStatemachine->l_color_index = par_l_index;
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;

    glob_pr_sglStatemachine->l_current_font_color_index = par_l_index;

    return;
}

/* End of File ***************************************************************/
