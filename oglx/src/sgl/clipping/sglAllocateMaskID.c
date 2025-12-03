/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglAllocateMaskID.c
 DESCRIPTION       : sglAllocateMaskID command shall return next available mask id for given type
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
  NAME: sglAllocateMaskID
  DESCRIPTION:
    Function shall return next available mask id for given type.
  SCOPE: global
  PARAMETERS:
    par_b_mask_kind -> kind of mask: SGL_TYPE_CLIP_PLANE, SGL_TYPE_CLIP_BOX or SGL_TYPE_STENCIL
  RETURN:
    SGLulong -> next available mask ID or SGL_NO_MASK
---------------------------------------------------------------------+*/
SGLulong sglAllocateMaskID(SGLbyte par_b_mask_kind)
{
    SGLulong loc_b_return = SGL_NO_MASK;
#ifndef OGLX_NO_GLCLIPPLANE
    SGLulong loc_ul_i = 0U;
#endif

    switch (par_b_mask_kind) {
#ifndef OGLX_NO_GLCLIPPLANE
    case SGL_TYPE_CLIP_PLANE:
        loc_ul_i = SGL_CLIP_PLANE0;
        while (((glob_pr_sglStatemachine->ul_allocated_masks & loc_ul_i) != 0x0) && (loc_ul_i <= SGL_CLIP_PLANE5)) {
            loc_ul_i = loc_ul_i << 1U;
        }

        if (loc_ul_i <= SGL_CLIP_PLANE5) {
            loc_b_return = loc_ul_i;
            glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
        }
        else {
            /* Nothing to do */
        }

        break;
#endif
    case SGL_TYPE_CLIP_BOX:
        if ((glob_pr_sglStatemachine->ul_allocated_masks & SGL_CLIP_BOX1) == 0x0) {
            loc_b_return = SGL_CLIP_BOX1;
            glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
        }
        else {
            if ((glob_pr_sglStatemachine->ul_allocated_masks & SGL_CLIP_BOX2) == 0x0) {
                loc_b_return = SGL_CLIP_BOX2;
                glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
            }
            else {
                /* Nothing to do */
            }
        }
        break;

    case SGL_TYPE_STENCIL:
        if ((glob_pr_sglStatemachine->ul_allocated_masks & SGL_STENCIL1) == 0x0) {
            loc_b_return = SGL_STENCIL1;
            glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
        }
        else {
            if ((glob_pr_sglStatemachine->ul_allocated_masks & SGL_STENCIL2) == 0x0) {
                loc_b_return = SGL_STENCIL2;
                glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
            }
            else {
                /* Nothing to do */
            }
        }
        break;
    default:
        oglxSetError(SGL_ERROR_SGL_ALLOCATEMASKID, 0U);
        break;
    }

    return loc_b_return;
}
