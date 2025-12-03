/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexFonti.c
 DESCRIPTION       : sglIndexFonti command shall select given font id
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
  NAME: sglIndexFonti
  DESCRIPTION:
    Function shall select given font id.
  SCOPE: global
  PARAMETERS:
    par_l_font_id -> Font index to select, range: [0,SGL_MAX_NUMBER_OF_FONTS-1]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglIndexFonti(SGLlong par_l_font_id)
{
    if ((par_l_font_id < (SGLlong) glob_pr_sglStatemachine->ul_nb_of_fonts) && (par_l_font_id >= 0L)) {
        glob_pr_sglStatemachine->ul_font_id = (SGLulong) par_l_font_id;
    }
    else {
        glob_pr_sglStatemachine->ul_font_id = INVALID_FONT_ID;
        oglxSetError(SGL_ERROR_SGL_INDEXFONTI, (SGLulong) par_l_font_id);
    }

    return;
}

/* End of File ***************************************************************/
