/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglFreeMaskID.c
 DESCRIPTION       : sglFreeMaskID command shall free given IDs through bitmask
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
  NAME: sglFreeMaskID
  DESCRIPTION:
    Function shall free given IDs through bitmask.
  SCOPE: global
  PARAMETERS:
    par_ul_mask_id -> IDs to free
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglFreeMaskID(SGLulong par_ul_mask_id)
{
    if ((par_ul_mask_id & SGL_UNUSED_MASK_BITS) != 0x0) {
        oglxSetError(SGL_ERROR_SGL_FREEMASKID, (SGLulong) par_ul_mask_id);
        par_ul_mask_id = par_ul_mask_id & (~SGL_UNUSED_MASK_BITS);
    }
    else {
        /* Nothing to do */
    }

    glob_pr_sglStatemachine->ul_allocated_masks &= ~par_ul_mask_id;

    if ((par_ul_mask_id & SGL_STENCIL1) == SGL_STENCIL1) {
        glob_pr_sglStatemachine->tr_stencil[0].b_define = SGL_FALSE;
        glob_pr_sglStatemachine->tr_stencil[0].b_state = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_ul_mask_id & SGL_STENCIL2) == SGL_STENCIL2) {
        glob_pr_sglStatemachine->tr_stencil[1].b_define = SGL_FALSE;
        glob_pr_sglStatemachine->tr_stencil[1].b_state = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_ul_mask_id & SGL_CLIP_BOX1) == SGL_CLIP_BOX1) {
        glob_pr_sglStatemachine->tr_clipbox[0].b_define = SGL_FALSE;
        glob_pr_sglStatemachine->tr_clipbox[0].b_state = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_ul_mask_id & SGL_CLIP_BOX2) == SGL_CLIP_BOX2) {
        glob_pr_sglStatemachine->tr_clipbox[1].b_define = SGL_FALSE;
        glob_pr_sglStatemachine->tr_clipbox[1].b_state = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    sglSetActiveMasks(glob_pr_sglStatemachine->ul_allocated_masks & glob_pr_sglStatemachine->ul_activated_masks);
    return;
}
