/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_stencil_buffer.c
 DESCRIPTION       : Set of functions used to manage stencil buffer
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
  NAME: sgl_stencil_compute_union_clipboxes
  DESCRIPTION:
    Function shall compute the union for two clipboxes which are visible inside.
  SCOPE: global
  PARAMETERS:
    void
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_stencil_compute_union_clipboxes(void)
{
    sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    void *loc_p_vertex_array;

    /* switch on the stencil test (maybe off due to no active box or stencil) */
    oglxEnable(GL_STENCIL_TEST);

    /* prevent GL from writing to the color and depth buffer */
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    /* open the corresponding stencil bits for writing the inside part */
    /* Select bit 4 -> 0x10 = 16 */
    glStencilMask((GLuint) 0x10);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    /* Save current modelview matrix */
#ifndef ES2_DEV_ENV
    glPushMatrix();
#else
    oglxPushMatrix();
#endif
    /* Disable texture arrays for stencil management */

    /* Clear with FFFF so external objects are not visible */
    glClearStencil(0x0000);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 0xFFFFl, 0x10U);

    /* Set the ortho for clipbox 1 */
#ifndef ES2_DEV_ENV
    glLoadMatrixf(glob_pr_sglStatemachine->tr_clipbox[0].pf_modelView);
#else
    oglxLoadMatrixf(glob_pr_sglStatemachine->tr_clipbox[0].pf_modelView);
#endif
    /* Send clipbox 1 to the Open GL stencil */
    loc_t_vertex_array[0][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_bottom_left_x;
    loc_t_vertex_array[0][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_bottom_left_y;
    loc_t_vertex_array[1][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_bottom_left_x;
    loc_t_vertex_array[1][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_top_right_y;
    loc_t_vertex_array[2][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_top_right_x;
    loc_t_vertex_array[2][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_top_right_y;
    loc_t_vertex_array[3][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_top_right_x;
    loc_t_vertex_array[3][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[0].f_bottom_left_y;

    loc_p_vertex_array = loc_t_vertex_array;

    /* Set the ortho for clipbox 2 */
#ifndef ES2_DEV_ENV
    oglxDrawVertexArray(GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
    glLoadMatrixf(glob_pr_sglStatemachine->tr_clipbox[1].pf_modelView);
#else
    oglxDrawVertexArray_es2(GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
    oglxLoadMatrixf(glob_pr_sglStatemachine->tr_clipbox[1].pf_modelView);
#endif

    /* Send clipbox 2 to the Open GL stencil */
    loc_t_vertex_array[0][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_bottom_left_x;
    loc_t_vertex_array[0][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_bottom_left_y;
    loc_t_vertex_array[1][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_bottom_left_x;
    loc_t_vertex_array[1][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_top_right_y;
    loc_t_vertex_array[2][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_top_right_x;
    loc_t_vertex_array[2][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_top_right_y;
    loc_t_vertex_array[3][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_top_right_x;
    loc_t_vertex_array[3][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[1].f_bottom_left_y;

    /* Restore current modelview matrix */
#ifndef ES2_DEV_ENV
    oglxDrawVertexArray(GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
    glPopMatrix();
#else
    oglxDrawVertexArray_es2(GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
    oglxPopMatrix();
#endif
    /* Allow drawing to color and depth buffer */
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilMask(0x0U);

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_stencils_mask
  DESCRIPTION:
    Function shall define mask function for stencils.
  SCOPE: global
  PARAMETERS:
    par_ul_mask -> Mask of the applied stencils, range: [0x0,0xFFFF]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_define_stencils_mask(SGLulong par_ul_mask)
{
    oglxEnable(GL_STENCIL_TEST);

#ifndef ES2_DEV_ENV
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
    glStencilFunc(GL_NOTEQUAL, 0x0000, (GLuint) par_ul_mask);
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_cb_and_stencil_mask
  DESCRIPTION:
    Function shall define mask function for clipbox 1 and one stencil.
  SCOPE: global
  PARAMETERS:          
    par_ul_mask -> Mask of the applied clipbox, range: [0x0,0xFFFF]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_define_cb_and_stencil_mask(SGLulong par_ul_mask)
{
    /* The intersection between the clipbox and the stencils is taken into account */
    /* Stencil 1 writes '1' in Stencil Buffer at bit 0 -> 0x1 */
    /* Stencil 2 writes '1' in Stencil Buffer at bit 1 -> 0x2 */
    /* Clipbox 1 writes '1' in Stencil Buffer at bit 2 -> 0x4 */

    /* They are both visible inside */
    oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
    glStencilFunc(GL_LEQUAL, (GLint) par_ul_mask, (GLuint) par_ul_mask);
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_cb1_and_both_stencil_mask
  DESCRIPTION:
    Function shall define mask function for clipbox 1 and both stencil.
  SCOPE: global
  PARAMETERS:
    par_ul_mask -> Mask of the applied clipbox, range: [0x0,0xFFFF]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_define_cb1_and_both_stencil_mask(SGLulong par_ul_mask)
{
    /* The intersection between the clipbox and the stencils is taken into account */
    /* Stencil 1 writes '1' in Stencil Buffer at bit 0 -> 0x1 */
    /* Stencil 2 writes '1' in Stencil Buffer at bit 1 -> 0x2 */
    /* Clipbox 1 writes '1' in Stencil Buffer at bit 2 -> 0x4 */

    oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
    /* Intersection between clip-box 1 and stencils corresponds to all values less than 0x4 */
    glStencilFunc(GL_LESS, 0x4, (GLuint) par_ul_mask);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_cb2_and_both_stencil_mask
  DESCRIPTION:
    Function shall define mask function for clipbox 2 and both stencil
  SCOPE: global
  PARAMETERS:
    par_ul_mask -> Mask of the applied clipbox, range: [0x0,0xFFFF]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_define_cb2_and_both_stencil_mask(SGLulong par_ul_mask)
{
    /* The intersection between the clipbox and the stencils is taken into account */
    /* Stencil 1 writes '1' in Stencil Buffer at bit 0 -> 0x1 */
    /* Stencil 2 writes '1' in Stencil Buffer at bit 1 -> 0x2 */
    /* Clipbox 2 writes '1' in Stencil Buffer at bit 3 -> 0x8 */


    /* They are both visible inside */
    oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
    /* Intersection between clip-box 2 and stencils corresponds to all values less than 0x8 */
    glStencilFunc(GL_LESS, 0x8, (GLuint) par_ul_mask);
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_both_clipbox_mask
  DESCRIPTION:
    Function shall define mask function for both clip box.
  SCOPE: global
  PARAMETERS:
    par_ul_mask -> Mask of the applied clipbox, range: [0x0,0xFFFF]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_define_both_clipbox_mask(SGLulong par_ul_mask)
{
    if (!(glob_pr_sglStatemachine->tr_clipbox[0].b_visible_outside)) {
        /*      Cb2     Cb1             Result */
        /*      1       1               visible */
        /*      1       0               visible */
        /*      0       1               visible */
        /*      0       0               not visible */
        /* They are both visible inside */
        oglxEnable(GL_STENCIL_TEST);

#ifndef ES2_DEV_ENV
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
        glStencilFunc(GL_NOTEQUAL, 0x0000, (GLuint) par_ul_mask);
    }
    else {
        /*      Cb2     Cb1             Result */
        /*      1       1               visible */
        /*      1       0               not visible */
        /*      0       1               not visible */
        /*      0       0               not visible */
        /* They are both visible outside */
        oglxEnable(GL_STENCIL_TEST);

#ifndef ES2_DEV_ENV
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
        glStencilFunc(GL_EQUAL, 0xFFFFl, (GLuint) par_ul_mask);
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_both_clipbox_and_stencil_mask
  DESCRIPTION:
    Function shall define mask function for both clip box and stencil.
  SCOPE: global
  PARAMETERS:
    par_ul_mask -> Applied mask, range: [0x0,0xFFFF]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_define_both_clipbox_and_stencil_mask(SGLulong par_ul_mask)
{
    if (!(glob_pr_sglStatemachine->tr_clipbox[0].b_visible_outside)) {
        /* Clipboxes are both visible inside */
        SGLulong loc_ul_mask_modify;

        sgl_stencil_compute_union_clipboxes();

        /* Modify the mask to take the bitplane 4 (-> 16) instead of bitplanes 3 (-> 8) and 2 (-> 4) */
        loc_ul_mask_modify = (par_ul_mask & 0x3UL) | 16UL;

        /* (/Cb2 | /Cb1) & (/St2 | /St1) */
        /*      Cb2     | Cb1   St2     St1             Case    Result */
        /*              0               0       0               0               visible */
        /*              0               0       1               1               visible */
        /*              0               1       0               2               visible */
        /*              0               1       1               3               not visible */
        /*      1               0   0       4           not visible */
        /*      1               0   1       5           not visible */
        /*      1               1   0       6           not visible */
        /*      1               1   1       7           not visible */

        if ((par_ul_mask & 0x3UL) == 0x3UL) {
            /* Stencil 1 and 2 active */
            oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
            /* Compare with mask of stencil 1 and 2 and bitplane 4 */
            glStencilFunc(GL_LEQUAL, 0x19, (GLuint) loc_ul_mask_modify);
        }
        else {
            /* Stencil 1 or 2 active */
            oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
            glStencilFunc(GL_LEQUAL, (GLint) loc_ul_mask_modify, (GLuint) loc_ul_mask_modify);
        }
    }
    else {
        /* (/Cb2 & /Cb1) & (/St2 | /St1) */
        /* Clipboxes are both visible outside */
        if ((par_ul_mask & 0x3UL) == 0x3UL) {
            /* Stencil 1 and 2 active */
            oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
            /* Compare with mask of clip box 1 and 2 all outside of it shall be visible */
            glStencilFunc(GL_LESS, 0xC, (GLuint) par_ul_mask);
        }
        else {
            oglxEnable(GL_STENCIL_TEST);
#ifndef ES2_DEV_ENV
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
            glStencilFunc(GL_LEQUAL, (GLint) par_ul_mask, (GLuint) par_ul_mask);
        }
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_stencil_buffer_Stencil_update
  DESCRIPTION:
    Function shall perform update of the stencil buffer.
  SCOPE: global
  PARAMETERS:
    void
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_stencil_buffer_Stencil_update(void)
{
    SGLulong loc_ul_mask;
    SGLulong loc_ul_tmp_mask;
    SGLulong loc_ul_stencil_mask = 0UL;
    SGLulong loc_ul_clipbox_mask = 0UL;

    SGLulong loc_ul_i;

    /* Update SW stencil 1 (bit 0) and stencil 2 (bit 1) */
    for (loc_ul_i = 0UL; loc_ul_i < 2UL; loc_ul_i++) {
        if (glob_pr_sglStatemachine->tr_stencil[loc_ul_i].b_state) {
            loc_ul_tmp_mask = 1UL;
            loc_ul_tmp_mask = loc_ul_tmp_mask << loc_ul_i;
            loc_ul_stencil_mask = loc_ul_stencil_mask | loc_ul_tmp_mask;
        }
        else {
            /* Nothing to do */
        }
    }


    /* Update Clipbox 1 (bit 2) and clipbox 2 (bit 3) */
    for (loc_ul_i = 0UL; loc_ul_i < 2UL; loc_ul_i++) {
        if (glob_pr_sglStatemachine->tr_clipbox[loc_ul_i].b_state) {
            loc_ul_tmp_mask = 1UL;
            loc_ul_tmp_mask = loc_ul_tmp_mask << (loc_ul_i + 2UL);
            loc_ul_clipbox_mask = loc_ul_clipbox_mask | loc_ul_tmp_mask;
        }
        else {
            /* Nothing to do */
        }
    }

    /* If both clipboxes are enabled */
    if (loc_ul_clipbox_mask == 12UL) {
        if ((glob_pr_sglStatemachine->tr_clipbox[0].b_visible_outside) && (!(glob_pr_sglStatemachine->tr_clipbox[1].b_visible_outside))) {
            /* Clipbox visible outside has a heigher priority than visible inside */
            /* Only the clipbox 1 should be considered, clipbox 2 is ignored */
            loc_ul_clipbox_mask = 4UL;
        }
        else {
            if ((!(glob_pr_sglStatemachine->tr_clipbox[0].b_visible_outside))
                && (glob_pr_sglStatemachine->tr_clipbox[1].b_visible_outside)) {
                /* Clipbox visible outside has a heigher priority than visible inside */
                /* Only the clipbox 2 should be considered, clipbox 1 is ignored */
                loc_ul_clipbox_mask = 8UL;
            }
            else {
                /* Nothing to do */
            }
        }
    }
    else {
        /* Nothing to do */
    }


    /* combine stencil and clipBox masks */
    loc_ul_mask = loc_ul_stencil_mask | loc_ul_clipbox_mask;

    switch (loc_ul_mask) {
    case 0UL:
    {
        oglxDisable(GL_STENCIL_TEST);
    }
        break;
    case 1UL:                  /* Only stencil 1 is enabled */
    case 2UL:                  /* Only stencil 2 is enabled */
    case 3UL:                  /* Both stencils are enabled */
    case 4UL:                  /* Only the clipbox 1 is enabled */
    case 8UL:                  /* Only the clipbox 2 is enabled */
        /*      St2     St1             Result */
        /*      0       0               visible */
        /*      0       1               visible */
        /*      1       0               visible */
        /*      1       1               not visible */
        sgl_define_stencils_mask(loc_ul_mask);
        break;
    case 5UL:                  /* Clipbox 1 and stencil 1 are enabled */
    case 6UL:                  /* Clipbox 1 and stencil 2 are enabled */
    case 9UL:                  /* Clipbox 2 and stencil 1 are enabled */
    case 10UL:                 /* Clipbox 2 and stencil 2 are enabled */
        sgl_define_cb_and_stencil_mask(loc_ul_mask);
        break;
    case 7UL:                  /* Clipbox 1 and stencils 1 & 2 are enabled */
        sgl_define_cb1_and_both_stencil_mask(loc_ul_mask);
        break;
    case 11UL:                 /* Clipbox 2 and stencils 1 & 2 are enabled */
        sgl_define_cb2_and_both_stencil_mask(loc_ul_mask);
        break;
    case 12UL:                 /* Clipboxes 1 & 2 are enabled: */
        /* they are either both visible inside or both visible outside */
        sgl_define_both_clipbox_mask(loc_ul_mask);
        break;
    default:                   /* Clipboxes 1 & 2 are enabled: */
        /* they are either both visible inside or both visible outside */
        /* Stencil 1 enabled */
        /* Stencil 2 enabled */
        /* Stencil 1 & 2 enabled */
        sgl_define_both_clipbox_and_stencil_mask(loc_ul_mask);
        break;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_stencil_buffer_ClipBox_description
  DESCRIPTION:
    Function shall set the clipboxes in the stencil buffer.
  SCOPE: global
  PARAMETERS:
    par_ul_clipboxID -> Clipbox identifier (SGL_CLIP_BOX1 or SGL_CLIP_BOX2)
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_stencil_buffer_ClipBox_description(SGLulong par_ul_clipboxID)
{
    sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;

    SGLulong loc_ul_mask = 0x1UL;
    void *loc_p_vertex_array;

    /* This function will only be called by sglClipBox, which has already
       check the validity of clip-box ID. No checked performed here. */

    /* Use mask 4 for clipbox 1 */
    /* Use mask 8 for clipbox 2 */
    loc_ul_mask = loc_ul_mask << (par_ul_clipboxID + 2UL);

    /* switch on the stencil test (maybe off due to no active box or stencil) */
    oglxEnable(GL_STENCIL_TEST);

    /* prevent GL from writing to the color and depth buffer */
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    /* open the corresponding stencil bits for writing the inside part */
    glStencilMask((GLuint) loc_ul_mask);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    /* Disable texture array for stencil management */

    if (glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].b_visible_outside) {
        /* Clipbox visible outside */
        /* Clear stencil is necessary for a stencil visible outside */
        glClearStencil(0xFFFFl);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0x0000l, (GLuint) loc_ul_mask);
    }
    else {
        /* No stencil was defined before or was visible outside */
        /* Clear stencil is necessary */
        /* Clipbox visible inside: Clear with FFFF so external objects are not visible */
        glClearStencil(0x0000);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, OGLX_MASK_FFFF, (GLuint) loc_ul_mask);
    }

    /* Send clipbox to the Open GL stencil */
#ifndef ES2_DEV_ENV
    glPushMatrix();
    glLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#else
    oglxPushMatrix();
    oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif
    loc_t_vertex_array[0][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_bottom_left_x;
    loc_t_vertex_array[0][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_bottom_left_y;
    loc_t_vertex_array[1][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_bottom_left_x;
    loc_t_vertex_array[1][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_top_right_y;
    loc_t_vertex_array[2][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_top_right_x;
    loc_t_vertex_array[2][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_top_right_y;
    loc_t_vertex_array[3][0] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_top_right_x;
    loc_t_vertex_array[3][1] = (GLfloat) glob_pr_sglStatemachine->tr_clipbox[par_ul_clipboxID].f_bottom_left_y;

    loc_p_vertex_array = loc_t_vertex_array;
#ifndef ES2_DEV_ENV
    oglxDrawVertexArray(GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
    glPopMatrix();
#else
    oglxDrawVertexArray_es2(GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
    oglxPopMatrix();
#endif

    /* Allow drawing to color and depth buffer */
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glStencilMask(0x0U);

    /* restore the valid stencil function */
    sgl_stencil_buffer_Stencil_update();

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_stencil_test
  DESCRIPTION:
    Function shall disable the stencil test.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_stencil_test(void)
{
    glDisable(GL_STENCIL_TEST);
    return;
}

/* End of File ***************************************************************/
