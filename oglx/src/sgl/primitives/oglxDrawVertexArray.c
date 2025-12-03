/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxDrawVertexArray.c
 DESCRIPTION       : Table command calls for vertices
 COPYRIGHT (C)     : 2010 by Esterel Technologies. All Rights Reserved.
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


#ifdef ES2_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxUpdateColor_es2
  DESCRIPTION:
    This function shall set current color in shaders.
  SCOPE: global
  PARAMETERS:
    par_pf_color -> The color to set in shaders (a table of 4 SGLfloat)
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxUpdateColor_es2(const SGLfloat * par_pf_color)
{
    sgl_flt_bin loc_prev;
    sgl_flt_bin loc_new;
    SGLfloat *loc_pr_current_color = &glob_pr_sglStatemachine->tf_current_color_es2[0];
    SGLbool loc_set = SGL_TRUE;

    /* Compare red component */
    loc_new.f = par_pf_color[0];
    loc_prev.f = loc_pr_current_color[0];
    if (loc_prev.l == loc_new.l) {
        /* Compare green component */
        loc_new.f = par_pf_color[1];
        loc_prev.f = loc_pr_current_color[1];
        if (loc_prev.l == loc_new.l) {
            /* Compare blue component */
            loc_new.f = par_pf_color[2];
            loc_prev.f = loc_pr_current_color[2];
            if (loc_prev.l == loc_new.l) {
                /* Compare alpha component */
                loc_new.f = par_pf_color[3];
                loc_prev.f = loc_pr_current_color[3];
                if (loc_prev.l == loc_new.l) {
                    /* There is no need to compare the alpha component */
                    loc_set = SGL_FALSE;
                }
            }
        }
    }

    if (loc_set) {
        loc_pr_current_color[0] = par_pf_color[0];
        loc_pr_current_color[1] = par_pf_color[1];
        loc_pr_current_color[2] = par_pf_color[2];
        loc_pr_current_color[3] = par_pf_color[3];
        glUniform4fv(glob_pr_sglStatemachine->ui_color_location, 1, par_pf_color);
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxDrawVertexArray_es2
  DESCRIPTION:
    Function shall call OpenGL commands to draw the figure 
    defined by given vertices and drawing mode.
  SCOPE: global
  PARAMETERS:
    par_e_mode -> Drawing mode (GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP)
    par_i_count  -> number of vertices, range: [2, SGL_MAX_VERTEX_ARRAY_SIZE]
    par_p_vertices -> vertices describing the figure
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxDrawVertexArray_es2(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices)
{
    /* Set color at index SGL_COLOR */
    oglxUpdateColor_es2(glob_pr_sglStatemachine->r_current_color.tf_current_color);
    oglxSetTextureState(0);
    /*
       Enable the custom vertex attribute at index OGLX_ES2_VERTEX_ARRAY.
       We previously binded that index to the variable in our shader "vec4 pf_vertex_array;"
     */

    if (glob_pr_sglStatemachine->b_tex_coord_array) {
        glDisableVertexAttribArray(OGLX_ES2_TEX_COORD_ARRAY);
        glob_pr_sglStatemachine->b_tex_coord_array = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    /* Sets the vertex data to this attribute index */
    if (glob_pr_sglStatemachine->p_current_vertex_array != par_pf_vertices) {
        glVertexAttribPointer(OGLX_ES2_VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, par_pf_vertices);
        glob_pr_sglStatemachine->p_current_vertex_array = (void *) par_pf_vertices;
    }
    else {
        /* Nothing to do */
    }

    glDrawArrays((GLenum) par_e_mode, 0, (GLsizei) par_i_count);
    return;
}
#else
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxDrawVertexArray
  DESCRIPTION:
    Function shall call OpenGL commands to draw the figure 
    defined by given vertices and drawing mode.
  SCOPE: global
  PARAMETERS:
    par_e_mode -> Drawing mode (GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP)
    par_i_count  -> number of vertices, range: [2, SGL_MAX_VERTEX_ARRAY_SIZE]
    par_p_vertices -> vertices describing the figure
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxDrawVertexArray(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices)
{
#ifndef OGLX_NO_VERTEX_ARRAY
#ifdef OGLX_DISPLAY_LISTS
    if (!(glob_pr_sglStatemachine->b_static_sequence_started)) {
#endif
        if (glob_pr_sglStatemachine->b_tex_coord_array) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glob_pr_sglStatemachine->b_tex_coord_array = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->p_current_vertex_array != par_pf_vertices) {
            glVertexPointer(2, GL_FLOAT, 0, par_pf_vertices);
            glob_pr_sglStatemachine->p_current_vertex_array = (void *) par_pf_vertices;
        }
        else {
            /* Nothing to do */
        }
        glDrawArrays((GLenum) par_e_mode, 0, (GLsizei) par_i_count);
#ifdef OGLX_DISPLAY_LISTS
    }
    else {
        SGLlong loc_l_i;
        glBegin((GLenum) par_e_mode);
        for (loc_l_i = 0; loc_l_i < (par_i_count * 2); loc_l_i += 2) {
            glVertex2f(par_pf_vertices[loc_l_i], par_pf_vertices[loc_l_i + 1]);
        }
        glEnd();
    }
#endif
#else
    SGLlong loc_l_i;
    glBegin((GLenum) par_e_mode);
    for (loc_l_i = 0; loc_l_i < (par_i_count * 2); loc_l_i += 2) {
        glVertex2f(par_pf_vertices[loc_l_i], par_pf_vertices[loc_l_i + 1]);
    }
    glEnd();
#endif
}
#endif
/* End of File ***************************************************************/
