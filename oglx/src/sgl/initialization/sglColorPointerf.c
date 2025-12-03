/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglColorPointerf.c
 DESCRIPTION       : sglColorPointerf shall define the colormap 
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
  NAME: sglColorPointerf
  DESCRIPTION:
    Function shall store the given colormap.
  SCOPE: global
  PARAMETERS:		
    par_pb_colormap -> Colormap            
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglColorPointerf(const sgl_color * par_ps_colormap, SGLulong par_ul_nb_colors)
{
    if ((par_ps_colormap != SGL_NULL) && (par_ul_nb_colors != 0) && (glob_pr_sglStatemachine->ul_currentAction == SGL_NO_VERTEX)) {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M11.5, 2, "Rule M11.5 is not applied here as glob_pr_sglStatemachine->tr_color content is never modified.")
#endif
        glob_pr_sglStatemachine->tr_color = (sgl_color *) par_ps_colormap;
        glob_pr_sglStatemachine->ul_nb_colors = par_ul_nb_colors;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_COLORPOINTERF, 0U);
    }

    return;
}

/* End of File ***************************************************************/
