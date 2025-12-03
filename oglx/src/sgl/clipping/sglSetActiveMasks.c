/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetActiveMasks.c
 DESCRIPTION       : sglSetActiveMasks command shall activate given masks
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
  NAME: sgl_enable_stencil
  DESCRIPTION:
    Function shall enable given stencil.
  SCOPE: local
  PARAMETERS:
    par_l_stencil_id -> stencil id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_enable_stencil(SGLlong par_l_stencil_id)
{
    if (glob_pr_sglStatemachine->tr_stencil[par_l_stencil_id].b_define) {
        if (!(glob_pr_sglStatemachine->tr_stencil[par_l_stencil_id].b_state)) {
            glob_pr_sglStatemachine->tr_stencil[par_l_stencil_id].b_state = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        if (par_l_stencil_id == 0) {
            oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) SGL_STENCIL1);
        }
        else {
            oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) SGL_STENCIL2);
        }
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_stencil
  DESCRIPTION:
    Function shall disable given stencil.
  SCOPE: local
  PARAMETERS:
    par_l_stencil_id -> stencil id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_stencil(SGLlong par_l_stencil_id)
{
    if (glob_pr_sglStatemachine->tr_stencil[par_l_stencil_id].b_define) {
        if (glob_pr_sglStatemachine->tr_stencil[par_l_stencil_id].b_state) {
            glob_pr_sglStatemachine->tr_stencil[par_l_stencil_id].b_state = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_enable_clip_box
  DESCRIPTION:
    Function shall enable given clip box.
  SCOPE: local
  PARAMETERS:
    par_l_clip_box_id -> clip box id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_enable_clip_box(SGLlong par_l_clip_box_id)
{
    if (glob_pr_sglStatemachine->tr_clipbox[par_l_clip_box_id].b_define) {
        if (!(glob_pr_sglStatemachine->tr_clipbox[par_l_clip_box_id].b_state)) {
            glob_pr_sglStatemachine->tr_clipbox[par_l_clip_box_id].b_state = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        if (par_l_clip_box_id == 0) {
            oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) SGL_CLIP_BOX1);
        }
        else {
            oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) SGL_CLIP_BOX2);
        }
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_clip_box
  DESCRIPTION:
    Function shall disable given clip box.
  SCOPE: local
  PARAMETERS:
    par_l_clip_box_id -> clip box id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_clip_box(SGLlong par_l_clip_box_id)
{
    if (glob_pr_sglStatemachine->tr_clipbox[par_l_clip_box_id].b_define) {
        if (glob_pr_sglStatemachine->tr_clipbox[par_l_clip_box_id].b_state) {
            glob_pr_sglStatemachine->tr_clipbox[par_l_clip_box_id].b_state = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetActiveMasks
  DESCRIPTION:
    Function shall activate given set of masks.
  SCOPE: global
  PARAMETERS:
    par_ul_mask_combination -> mask combination
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetActiveMasks(SGLulong par_ul_mask_combination)
{
    if ((par_ul_mask_combination & SGL_UNUSED_MASK_BITS) != 0x0) {
        oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) par_ul_mask_combination);
    }
    else {
        /* Nothing to do */
    }

    if (glob_pr_sglStatemachine->ul_activated_masks != par_ul_mask_combination) {
        glob_pr_sglStatemachine->ul_activated_masks = par_ul_mask_combination;

#ifndef OGLX_NO_GLCLIPPLANE
        if ((par_ul_mask_combination & SGL_CLIP_PLANE0) == SGL_CLIP_PLANE0) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE0);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE0);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE1) == SGL_CLIP_PLANE1) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE1);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE1);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE2) == SGL_CLIP_PLANE2) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE2);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE2);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE3) == SGL_CLIP_PLANE3) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE3);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE3);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE4) == SGL_CLIP_PLANE4) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE4);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE4);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE5) == SGL_CLIP_PLANE5) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE5);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE5);
        }
#endif
        if ((par_ul_mask_combination & SGL_STENCIL1) == SGL_STENCIL1) {
            sgl_enable_stencil(0);
        }
        else {
            sgl_disable_stencil(0);
        }

        if ((par_ul_mask_combination & SGL_STENCIL2) == SGL_STENCIL2) {
            sgl_enable_stencil(1);
        }
        else {
            sgl_disable_stencil(1);
        }

        if ((par_ul_mask_combination & SGL_CLIP_BOX1) == SGL_CLIP_BOX1) {
            sgl_enable_clip_box(0);
        }
        else {
            sgl_disable_clip_box(0);
        }

        if ((par_ul_mask_combination & SGL_CLIP_BOX2) == SGL_CLIP_BOX2) {
            sgl_enable_clip_box(1);
        }
        else {
            sgl_disable_clip_box(1);
        }

        /* Update stencil buffer in case stencil function has been updated */
        sgl_stencil_buffer_Stencil_update();
        glob_pr_sglStatemachine->b_stencil_update = SGL_FALSE;
    }
    else {
        if (glob_pr_sglStatemachine->b_stencil_update) {
            /* Update stencil buffer in case stencil function has been updated */
            sgl_stencil_buffer_Stencil_update();
            glob_pr_sglStatemachine->b_stencil_update = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }
    }

    return;
}
