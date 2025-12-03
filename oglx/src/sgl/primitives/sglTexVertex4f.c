/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglTexVertex4f.c
 DESCRIPTION       : sglTexVertex4f command shall specify a vertex with its texture coordinates
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
#include "mth.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxTexVertex4f
  DESCRIPTION:
    Function shall specify a vertex and its associated texture coordinates.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the vertex
    par_f_y -> y-coordinate of the vertex
    par_f_u -> u-coordinate of cuurent texture applied to this vertex
    par_f_v -> v-coordinate of cuurent texture applied to this vertex
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxTexVertex4f(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_u, SGLfloat par_f_v)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

    if (loc_l_size < (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE) {
        SGLfloat loc_f_u;
        SGLfloat loc_f_v;

        loc_f_u = (glob_pr_sglStatemachine->f_current_dx + glob_pr_sglStatemachine->f_current_nx)
            * par_f_u;
        loc_f_v = (glob_pr_sglStatemachine->f_current_dy + glob_pr_sglStatemachine->f_current_ny)
            * par_f_v;

        sglTexCoord2f(loc_f_u, loc_f_v);

        /* insert new vertex */
        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][0]
            = par_f_x;

        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][1]
            = par_f_y;

        /* set new size */
        glob_pr_sglStatemachine->tr_geometry.l_size = loc_l_size + 1;

        /* Save the first vertex */
        glob_pr_sglStatemachine->f_line_loop_first_vertex_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0];
        glob_pr_sglStatemachine->f_line_loop_first_vertex_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1];
    }
    else {
        oglxSetError(SGL_ERROR_SGL_TEXVERTEX4F, 0U);
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglTexVertex4f
  DESCRIPTION:
    Function shall specify a vertex and its associated texture coordinates.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the vertex
    par_f_y -> y-coordinate of the vertex
    par_f_u -> u-coordinate of cuurent texture applied to this vertex
    par_f_v -> v-coordinate of cuurent texture applied to this vertex
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglTexVertex4f(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_u, SGLfloat par_f_v)
{
    if (((glob_pr_sglStatemachine->ul_currentAction == SGL_POLYGON)
         || (glob_pr_sglStatemachine->ul_currentAction == SGL_TRIANGLE_STRIP))
        && glob_pr_sglStatemachine->b_texture_state) {
        oglxTexVertex4f(par_f_x, par_f_y, par_f_u, par_f_v);

        glob_pr_sglStatemachine->b_new_vertex = SGL_TRUE;
    }
    else {
        /* Raise error: sglTexVertex4f called outside a sglBegin/sglEnd block */
        oglxSetError(SGL_ERROR_SGL_TEXVERTEX4F, 0U);
    }

    return;
}

/* End of File ***************************************************************/
