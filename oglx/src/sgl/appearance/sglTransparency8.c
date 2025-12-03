/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglTransparency8.c
 DESCRIPTION       : sglTransparency8 command shall set the alpha value for 
					transparency on filled shapes and on lines
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
  NAME: sglTransparency8
  DESCRIPTION:
    Function shall set the alpha value for transparency 
    on filled shapes and on lines.
  SCOPE: global
  PARAMETERS:
    par_l_alpha -> alpha value, range: [0,255] 
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglTransparency8(SGLlong par_l_alpha)
{
    if ((par_l_alpha >= 0L) && (par_l_alpha <= 255L) && (glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX)) {
        glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;
        glob_pr_sglStatemachine->b_transparency = (SGLbyte) par_l_alpha;        /* code the value on 8 bits */
    }
    else {
        oglxSetError(SGL_ERROR_SGL_TRANSPARENCY8, (SGLulong) par_l_alpha);
    }

    return;
}

/* End of File ***************************************************************/
