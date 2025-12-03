/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglConvertPointToScreen.c
 DESCRIPTION       : sglConvertPointToScreen command shall convert the given user coordinates to screen coordinates
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
  NAME: sglConvertPointToScreen
  DESCRIPTION:
    Function shall convert the given user coordinates to screen coordinates
  SCOPE: global
  PARAMETERS:
    par_f_x -> X-coordinate in user units
    par_f_y -> Y-coordinate in user units
    par_pf_screen_x -> X-coordinate in screen units
    par_pf_screen_y -> Y-coordinate in screen units
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglConvertPointToScreen(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_screen_x, SGLfloat * par_pf_screen_y)
{

    sglConvertPointToPixel(par_f_x, par_f_y, par_pf_screen_x, par_pf_screen_y);
    *par_pf_screen_x =
        SGLfloat_div(*par_pf_screen_x, ((2.0F * glob_pr_sglStatemachine->f_viewport_width_div_2) / ((SGLfloat) glob_pr_sglStatemachine->ul_screen_w)));
    *par_pf_screen_y =
        SGLfloat_div(*par_pf_screen_y, ((2.0F * glob_pr_sglStatemachine->f_viewport_height_div_2) / ((SGLfloat) glob_pr_sglStatemachine->ul_screen_h)));

    return;
}
