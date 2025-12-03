/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBegin.c
 DESCRIPTION       : sglBegin shall begin the vertices 
					of a primitive or a group of primitives.
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
  NAME: sgl_stencil_buffer_Stencil_begin
  DESCRIPTION:
    Function shall begin the stencil description.
  SCOPE: local
  PARAMETERS:          
    par_ul_stencilID -> stencil to describe
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_stencil_buffer_Stencil_begin(SGLulong par_ul_stencilID)
{
    SGLulong loc_ul_mask = 0x1UL;

    /* This function will only be called by sglBegin, which will always
       use a valid stencil ID. No checked performed here. */

    loc_ul_mask = loc_ul_mask << par_ul_stencilID;

    /* switch on the stencil test (maybe off due to no active box or stencil) */
    oglxEnable(GL_STENCIL_TEST);

    /* prevent GL from writing to the color and depth buffer */
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    /* open the corresponding stencil bits for writing the stencil */
    glStencilMask((GLuint) loc_ul_mask);

    /* Clear the old stencil */
    glClearStencil(0x0000);
    glClear(GL_STENCIL_BUFFER_BIT);

    /* Set the stenciled area to 0 */
    glStencilFunc(GL_ALWAYS, OGLX_MASK_FFFF, (GLuint) loc_ul_mask);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    glob_pr_sglStatemachine->b_stencil_update = SGL_TRUE;

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_set_line_mode
  DESCRIPTION:
    Function shall disable texturing and set line mode.
  SCOPE: local
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_line_mode(void)
{
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_LINE;

#ifndef ES2_DEV_ENV
    oglxDisable(GL_TEXTURE_2D);
#endif

    oglxEnable(GL_BLEND);

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_set_fill_mode
  DESCRIPTION:
    Function shall enable texturing if a texture is a filled
    shape and disable line mode.
  SCOPE: local
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_fill_mode(void)
{
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_FILL;

    if (glob_pr_sglStatemachine->b_halo_level_state) {
        oglxDisable(GL_DEPTH_TEST);
    }

    oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);

    /*  If texture state is enabled and the texture bound has a correct ID */
    if ((loc_l_last_texture_bound_index < (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures)
        && (glob_pr_sglStatemachine->b_texture_state)) {

        if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
            oglxBindTexture(glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].ui_texture);

            /* The bound texture is not the Simple Font texture */
            if ((glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_RGB_TRANSPARENT)
                || (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA)) {
                oglxEnable(GL_BLEND);
            }
            else {
                oglxDisable(GL_BLEND);
            }

#ifndef ES2_DEV_ENV
            oglxEnable(GL_TEXTURE_2D);
#endif
        }
    }
    else {
#ifndef ES2_DEV_ENV
        oglxDisable(GL_TEXTURE_2D);
#endif
        oglxEnable(GL_BLEND);
    }

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglBegin
  DESCRIPTION:
    Function shall set the type of the primitive.
  SCOPE: global
  PARAMETERS:
    par_b_mode ->  specification of primitives that will
    be created from vertices declared between sglBegin and 
    sglEnd. Values: SGL_LINES,SGL_LINE_STRIP, SGL_LINE_LOOP,
    SGL_POLYGON, SGL_STENCIL1 or
    SGL_STENCIL2
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglBegin(SGLbyte par_b_mode)
{
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;
    glob_pr_sglStatemachine->b_new_vertex = SGL_FALSE;

    if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
        if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
            loc_b_texture_alpha = SGL_TRUE;
        }
    }

    /* Nothing shall be drawn if a texture of type SGL_BITMAP_ALPHA is defined and if modulate is inactive */
    switch (par_b_mode) {
    case SGL_LINES:
    {
        /* Update OGLX context to prepare a draw in SGL_LINES mode, it shall draw lines */
        sgl_set_line_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;
        glob_pr_sglStatemachine->b_sgl_lines = SGL_TRUE;
    }
        break;
    case SGL_LINE_STRIP:
    {
        /* Update OGLX context to prepare a draw in SGL_LINE_STRIP mode, it is a line mode */
        sgl_set_line_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINE_STRIP;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;
    }
        break;
    case SGL_LINE_LOOP:
    {
        /* Update OGLX context to prepare a draw in SGL_LINE_LOOP mode, it is a line mode */
        sgl_set_line_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINE_LOOP;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        glob_pr_sglStatemachine->b_close_line_loop = SGL_TRUE;
    }
        break;
    case SGL_TRIANGLE_STRIP:
    {
        if ((glob_pr_sglStatemachine->b_texture_state) && loc_b_texture_alpha && !glob_pr_sglStatemachine->b_modulate) {
            /* Nothing to do */
        }
        else {
            /* Update OGLX context to prepare a draw in SGL_TRIANGLE_STRIP mode, it is a filled mode */
            sgl_set_fill_mode();
            glob_pr_sglStatemachine->ul_currentAction = SGL_TRIANGLE_STRIP;
            /* Set Geometry type to SGL_LINE_LOOP for surrounding line drawing of polygon smooth */
            glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        }
    }
        break;
    case SGL_POLYGON:
    {
        if ((glob_pr_sglStatemachine->b_texture_state) && loc_b_texture_alpha && !glob_pr_sglStatemachine->b_modulate) {
            /* Nothing to do */
        }
        else {
            /* Update OGLX context to prepare a draw in SGL_POLYGON mode, it is a filled mode */
            sgl_set_fill_mode();
            glob_pr_sglStatemachine->ul_currentAction = SGL_POLYGON;
            /* Set Geometry type to SGL_LINE_LOOP for surrounding line drawing of polygon smooth */
            glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        }
    }
        break;
    case SGL_STENCIL1:
    {
        /* Update OGLX context to prepare a draw in SGL_STENCIL1 mode */
        sgl_stencil_buffer_Stencil_begin(0UL);
        glob_pr_sglStatemachine->ul_currentAction = SGL_STENCIL1;
    }
        break;
    case SGL_STENCIL2:
    {
        /* Update OGLX context to prepare a draw in SGL_STENCIL2 mode */
        sgl_stencil_buffer_Stencil_begin(1UL);
        glob_pr_sglStatemachine->ul_currentAction = SGL_STENCIL2;
    }
        break;
    default:
    {
        /* In default case, no drawing mode is required */
        glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
        oglxSetError(SGL_ERROR_SGL_BEGIN, (SGLulong) par_b_mode);
    }
        break;
    }
    glob_pr_sglStatemachine->tr_geometry.l_size = 0;

    return;
}

/* End of File ***************************************************************/
