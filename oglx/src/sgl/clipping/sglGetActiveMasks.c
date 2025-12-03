/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetActiveMasks.c
 DESCRIPTION       : sglGetActiveMasks command shall return active masks
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
  NAME: sglGetActiveMasks
  DESCRIPTION:
    Function shall return active masks.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    void
---------------------------------------------------------------------+*/
SGLulong sglGetActiveMasks(void)
{
    return glob_pr_sglStatemachine->ul_activated_masks;
}
