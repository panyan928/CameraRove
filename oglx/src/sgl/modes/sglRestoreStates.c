/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglRestoreStates.c
 DESCRIPTION       : sglRestoreStates command shall restore all OpenGL states set by OGLX in case it has been changed by an external OpenGL call.
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
  NAME: sglRestoreStates
  DESCRIPTION:
    sglRestoreStates command shall restore all OpenGL states set by OGLX in case it has been changed by an external OpenGL call.
  SCOPE: global
  PARAMETERS:
    None            
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglRestoreStates(void)
{
    void *loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    void *loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    glob_pr_sglStatemachine->p_current_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;

#ifndef ES2_DEV_ENV
    glMatrixMode(GL_MODELVIEW);
    /* Restore GL_TEXTURE_2D state */
    if (glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
#endif

    /* Restore GL_BLEND state */
    if (glob_pr_sglStatemachine->r_ogl_state.b_state_blend) {
        glEnable(GL_BLEND);
    }
    else {
        glDisable(GL_BLEND);
    }

    /* Restore GL_STENCIL_TEST state */
    if (glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test) {
        glEnable(GL_STENCIL_TEST);
    }
    else {
        glDisable(GL_STENCIL_TEST);
    }

    /* Restore GL_SCISSOR_TEST state */
    if (glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test) {
        glEnable(GL_SCISSOR_TEST);
    }
    else {
        glDisable(GL_SCISSOR_TEST);
    }

    if (glob_pr_sglStatemachine->ui_ogl_texture_bound != NO_OGL_TEXTURE_BOUND) {
        glBindTexture(GL_TEXTURE_2D, glob_pr_sglStatemachine->ui_ogl_texture_bound);
    }
    else {
        /* Nothing to do */
    }

    if (!sgluFloatIsEqual(glob_pr_sglStatemachine->f_last_ogl_linewidth, 0.0F)) {
        glLineWidth(glob_pr_sglStatemachine->f_last_ogl_linewidth *
                    ((glob_pr_sglStatemachine->f_width_factor + glob_pr_sglStatemachine->f_height_factor) * 0.5F));
    }

#ifndef ES2_DEV_ENV
    if (glob_pr_sglStatemachine->b_modulate) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_MODULATE);
    }
    else {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
    }
#ifndef OGLX_NO_VERTEX_ARRAY
    if (glob_pr_sglStatemachine->b_tex_coord_array) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    else {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
#endif
#else
    if (glob_pr_sglStatemachine->b_tex_coord_array) {
        glEnableVertexAttribArray(OGLX_ES2_TEX_COORD_ARRAY);
    }
    else {
        glDisableVertexAttribArray(OGLX_ES2_TEX_COORD_ARRAY);
    }
#endif
    /* Restore last color set */
#ifndef ES2_DEV_ENV
    glColor4f(glob_pr_sglStatemachine->r_current_color.tf_current_color[0],
              glob_pr_sglStatemachine->r_current_color.tf_current_color[1],
              glob_pr_sglStatemachine->r_current_color.tf_current_color[2], glob_pr_sglStatemachine->r_current_color.tf_current_color[3]);
#endif

#ifndef ES2_DEV_ENV
#ifndef OGLX_NO_VERTEX_ARRAY
    glTexCoordPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_vertex_array);
#endif
#else
    glVertexAttribPointer(OGLX_ES2_TEX_COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexAttribPointer(OGLX_ES2_VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_vertex_array);
#endif
    return;
}

/* End of File ***************************************************************/
