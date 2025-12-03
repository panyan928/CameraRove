/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetLineCaps.c
 DESCRIPTION       : sglSetLineCaps command shall store the line caps appearance
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
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetLineCaps
  DESCRIPTION:
    Function shall set the line caps rendering style.
  SCOPE: global
  PARAMETERS:
    par_l_type -> the type of the line (SGL_STRAIGHT_CAPS or SGL_ROUNDED_CAPS)
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetLineCaps(SGLlong par_l_type)
{
    if (par_l_type == SGL_STRAIGHT_CAPS) {
        glob_pr_sglStatemachine->l_line_caps_appearance = SGL_STRAIGHT_CAPS;
    }
    else {
        glob_pr_sglStatemachine->l_line_caps_appearance = SGL_ROUNDED_CAPS;
    }

    return;
}
