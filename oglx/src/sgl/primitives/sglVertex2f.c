/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglVertex2f.c
 DESCRIPTION       : sglVertex2f command shall specify a vertex
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
  NAME: sglTexCoord2f
  DESCRIPTION:
    Function shall set the texture coordinates for the SGL textures.
  SCOPE: local
  PARAMETERS:
    par_f_u -> texture u-coordinate
    par_f_v -> texture v-coordinate
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglTexCoord2f(SGLfloat par_f_u, SGLfloat par_f_v)
{
    switch (glob_pr_sglStatemachine->ul_currentAction) {
    case SGL_TRIANGLE_STRIP:
    case SGL_POLYGON:
    {
        SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord;
        if (loc_l_size < (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE) {

            /* Fill texture coordinates table */
            glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_l_size][0] = (SGLfloat) par_f_u;
            glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_l_size][1] = (SGLfloat) par_f_v;

            glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord++;

        }
        else {
            glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0;
        }
    }
        break;
    default:
        break;
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_vertex_2f_add_line
  DESCRIPTION:
    Function shall add a simple line in vertex array.
  SCOPE: global
  PARAMETERS:
    par_f_x1 -> x-parameter of the start point
    par_f_y1 -> y-parameter of the start point
    par_f_x2 -> x-parameter of the end point
    par_f_y2 -> y-parameter of the end point
  RETURN:
    SGL_TRUE, if the maximum number of vertices is not reached, SGL_FALSE otherwise.
---------------------------------------------------------------------+*/
SGLbool sgl_vertex_2f_add_line(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLbool loc_b_return = SGL_TRUE;

    if ((loc_l_size + 2L) < (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE) {
        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][0] = par_f_x1;
        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][1] = par_f_y1;
        loc_l_size++;

        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][0] = par_f_x2;
        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][1] = par_f_y2;
        glob_pr_sglStatemachine->tr_geometry.l_size += 2;
    }
    else {
        loc_b_return = SGL_FALSE;
    }

    return loc_b_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_vertex_2f_line_stipple
  DESCRIPTION:
    Function shall define vertices for line stipples.
  SCOPE: global
  PARAMETERS:
    par_f_x1 -> x-parameter of the start point
    par_f_y1 -> y-parameter of the start point
    par_f_x2 -> x-parameter of the end point
    par_f_y2 -> y-parameter of the end point
    par_ul_index -> linestipple index, range: [0,5]
  RETURN:
    SGLulong -> last index of linestippling, range: [0,5](0 in case of SGL_LINES)
---------------------------------------------------------------------+*/
SGLulong sgl_vertex_2f_line_stipple(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2, SGLulong par_ul_index)
{
    SGLfloat loc_f_dx_norm;
    SGLfloat loc_f_dx;
    SGLfloat loc_f_dy_norm;
    SGLfloat loc_f_dy;
    SGLfloat loc_f_norm;
    SGLfloat loc_f_norm_segment;
    SGLfloat loc_f_dx_segment;
    SGLfloat loc_f_dy_segment;
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_y;

    SGLfloat loc_f_x1 = par_f_x1;
    SGLfloat loc_f_y1 = par_f_y1;
    SGLfloat loc_f_x2;
    SGLfloat loc_f_y2;
    SGLbool loc_b_continue = SGL_TRUE;

    SGLfloat loc_f_current_length;

    SGLbyte loc_b_nb_pattern = glob_pr_sglStatemachine->pr_stipple->b_number;

    SGLulong loc_ul_return = 0UL;

    loc_f_dx = par_f_x2 - par_f_x1;
    loc_f_dy = par_f_y2 - par_f_y1;

    if (sgluFloatIsEqual(0.0F, loc_f_dx)) {
        loc_f_norm = mth_f_abs(loc_f_dy);

        loc_f_dy_segment = SGLfloat_div(loc_f_dy, loc_f_ratio_y);
        loc_f_dx_segment = 0.0F;
        loc_f_norm_segment = mth_f_abs(loc_f_dy_segment);
    }
    else if (sgluFloatIsEqual(0.0F, loc_f_dy)) {
        loc_f_norm = mth_f_abs(loc_f_dx);

        loc_f_dx_segment = SGLfloat_div(loc_f_dx, loc_f_ratio_x);
        loc_f_dy_segment = 0.0F;
        loc_f_norm_segment = mth_f_abs(loc_f_dx_segment);
    }
    else {
        loc_f_norm = (loc_f_dx * loc_f_dx) + (loc_f_dy * loc_f_dy);
        loc_f_norm = mth_sqrtf(loc_f_norm);

        /* Compute total length of segment in pixels */
        loc_f_dx_segment = SGLfloat_div(loc_f_dx, loc_f_ratio_x);
        loc_f_dy_segment = SGLfloat_div(loc_f_dy, loc_f_ratio_y);
        loc_f_norm_segment = (loc_f_dx_segment * loc_f_dx_segment) + (loc_f_dy_segment * loc_f_dy_segment);
        loc_f_norm_segment = mth_sqrtf(loc_f_norm_segment);
    }


    /* No necessary check on par_ul_index as in the first call the value is zero 
       and the value will be checked in this function when it is incremented. */

    loc_f_current_length = (SGLfloat) glob_pr_sglStatemachine->pr_stipple->tb_length[par_ul_index] - glob_pr_sglStatemachine->f_last_stipple_length;

    if (loc_f_norm > SGL_MIN_F) {
        loc_f_dx_norm = SGLfloat_div(loc_f_dx * glob_pr_sglStatemachine->f_ratio_scale_x, loc_f_norm);
        loc_f_dy_norm = SGLfloat_div(loc_f_dy * glob_pr_sglStatemachine->f_ratio_scale_y, loc_f_norm);
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    }
    else {
        loc_f_dx_norm = 1.0F;
        loc_f_dy_norm = 0.0F;
    }

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    loc_f_dx = mth_f_abs(loc_f_dx);

    loc_f_x2 = (loc_f_current_length * loc_f_dx_norm) + par_f_x1;
    loc_f_y2 = (loc_f_current_length * loc_f_dy_norm) + par_f_y1;

    while ((loc_f_current_length <= loc_f_norm_segment) && loc_b_continue) {
        if (glob_pr_sglStatemachine->pr_stipple->tb_on_off[par_ul_index] == 1U) {
            loc_b_continue = sgl_vertex_2f_add_line(loc_f_x1, loc_f_y1, loc_f_x2, loc_f_y2);
        }
        else {
            /* Nothing to do */
        }

        par_ul_index++;
        if (par_ul_index >= (SGLulong) loc_b_nb_pattern) {
            par_ul_index = 0UL;
        }
        else {
            /* Nothing to do */
        }

        loc_f_current_length += (SGLfloat) glob_pr_sglStatemachine->pr_stipple->tb_length[par_ul_index];

        loc_f_x1 = loc_f_x2;
        loc_f_y1 = loc_f_y2;

        loc_f_x2 = (loc_f_current_length * loc_f_dx_norm) + par_f_x1;
        loc_f_y2 = (loc_f_current_length * loc_f_dy_norm) + par_f_y1;
    }

    /* last stipple */
    if (glob_pr_sglStatemachine->pr_stipple->tb_on_off[par_ul_index] == 1U) {
        loc_b_continue = sgl_vertex_2f_add_line(loc_f_x1, loc_f_y1, par_f_x2, par_f_y2);
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_continue) {
        oglxSetError(SGL_ERROR_SGL_VERTEX2F, 0U);
    }
    else {
        /* Nothing to do */
    }

    loc_ul_return = par_ul_index;

    /* Compute total length of segment in pixels */
    glob_pr_sglStatemachine->f_last_stipple_length =
        (SGLfloat) glob_pr_sglStatemachine->pr_stipple->tb_length[par_ul_index] - (loc_f_current_length - loc_f_norm_segment);

    return loc_ul_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxSetTextureCoordinates
  DESCRIPTION:
    Function shall set texture coordinates.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the vertex
    par_f_y -> y-coordinate of the vertex
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxSetTextureCoordinates(SGLfloat par_f_x, SGLfloat par_f_y)
{
    SGLfloat loc_f_u_factor = 1.0F;
    SGLfloat loc_f_v_factor = 1.0F;
    SGLfloat loc_f_u;
    SGLfloat loc_f_v;
    SGLlong loc_l_texture_index = glob_pr_sglStatemachine->l_last_texture_bound_index;

    /* Robustness case when sglBindTexture has never been called */
    if (loc_l_texture_index < (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) {
        if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
            if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_texture_index].b_textures_app_mode == SGL_CLAMP) {
                loc_f_u_factor =
                    SGLfloat_div((SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[loc_l_texture_index].ul_textures_dimension[0],
                                 (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[loc_l_texture_index].ul_dimension_power_2[0]);
                loc_f_v_factor =
                    SGLfloat_div((SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[loc_l_texture_index].ul_textures_dimension[1],
                                 (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[loc_l_texture_index].ul_dimension_power_2[1]);
            }
            else {
                /* Nothing to do */
            }
        }
        if (sgluFloatIsEqual(glob_pr_sglStatemachine->f_current_bbox_width, 0.0F)) {
            loc_f_u = loc_f_u_factor;
        }
        else {
            loc_f_u = (glob_pr_sglStatemachine->f_current_dx + glob_pr_sglStatemachine->f_current_nx
                       * SGLfloat_div(par_f_x - glob_pr_sglStatemachine->f_current_xmin, glob_pr_sglStatemachine->f_current_bbox_width)) * loc_f_u_factor;
        }

        if (sgluFloatIsEqual(glob_pr_sglStatemachine->f_current_bbox_height, 0.0F)) {
            loc_f_v = loc_f_v_factor;
        }
        else {
            loc_f_v = (glob_pr_sglStatemachine->f_current_dy + glob_pr_sglStatemachine->f_current_ny
                       * SGLfloat_div(par_f_y - glob_pr_sglStatemachine->f_current_ymin, glob_pr_sglStatemachine->f_current_bbox_height)) * loc_f_v_factor;
        }

        sglTexCoord2f(loc_f_u, loc_f_v);
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxVertex2f
  DESCRIPTION:
    Function shall specify a vertex.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the vertex
    par_f_y -> y-coordinate of the vertex
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxVertex2f(SGLfloat par_f_x, SGLfloat par_f_y)
{
    SGLlong loc_l_size;

    if (glob_pr_sglStatemachine->b_texture_state) {
        oglxSetTextureCoordinates(par_f_x, par_f_y);
    }
    else {
        /* Nothing to do */
    }

    loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

    if (loc_l_size < (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE) {

        if ((!(glob_pr_sglStatemachine->b_switch_to_sgl_lines)) || (loc_l_size == 0)) {
            /* Manage line stipple when initial mode is SGL_LINES */
            if ((glob_pr_sglStatemachine->ul_currentAction == SGL_LINES)
                && (!(glob_pr_sglStatemachine->b_drawing_arc))
                && (glob_pr_sglStatemachine->b_stipple_ok)) {
                if (!(glob_pr_sglStatemachine->b_odd_vertex)) {
                    glob_pr_sglStatemachine->b_odd_vertex = SGL_TRUE;
                }
                else {
                    /* Define the line stipple vertices */
                    glob_pr_sglStatemachine->ul_last_stipple_index =
                        sgl_vertex_2f_line_stipple(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y, par_f_x, par_f_y,
                                                   glob_pr_sglStatemachine->ul_last_stipple_index);
                    glob_pr_sglStatemachine->b_odd_vertex = SGL_FALSE;
                }
            }
            else {
                /* insert new vertex */
                glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][0]
                    = par_f_x;

                glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size][1]
                    = par_f_y;

                /* set new size */
                glob_pr_sglStatemachine->tr_geometry.l_size = loc_l_size + 1;
            }
        }
        else {
            /* Define the line stipple vertices */
            glob_pr_sglStatemachine->ul_last_stipple_index =
                sgl_vertex_2f_line_stipple(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y, par_f_x, par_f_y,
                                           glob_pr_sglStatemachine->ul_last_stipple_index);
        }

        /* last point shall be stored except if an arc is currently drawn */
        if (glob_pr_sglStatemachine->b_store_last_point) {
            glob_pr_sglStatemachine->f_last_point_x = par_f_x;
            glob_pr_sglStatemachine->f_last_point_y = par_f_y;
        }
        else {
            /* Nothing to do */
        }

        /* Save the first vertex */
        glob_pr_sglStatemachine->f_line_loop_first_vertex_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0];
        glob_pr_sglStatemachine->f_line_loop_first_vertex_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1];
    }
    else {
        oglxSetError(SGL_ERROR_SGL_VERTEX2F, 0U);
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglVertex2f
  DESCRIPTION:
    Function shall specify a vertex.
  SCOPE: global
  PARAMETERS:
    par_f_x -> x-coordinate of the vertex
    par_f_y -> y-coordinate of the vertex
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglVertex2f(SGLfloat par_f_x, SGLfloat par_f_y)
{

    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;

    if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
        if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
            loc_b_texture_alpha = SGL_TRUE;
        }
    }
    /* Nothing shall be drawn if a texture of type SGL_BITMAP_ALPHA is defined and if modulate is inactive */
    if ((glob_pr_sglStatemachine->b_texture_state) && loc_b_texture_alpha && !glob_pr_sglStatemachine->b_modulate) {
        /* Nothing to do */
    }
    else {

        if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
            if (glob_pr_sglStatemachine->b_sgl_lines) {
                glob_pr_sglStatemachine->f_last_stipple_length = 0.0F;
                glob_pr_sglStatemachine->ul_last_stipple_index = 0UL;
            }
            else {
                /* Nothing to do */
            }

            if ((glob_pr_sglStatemachine->b_stipple_ok)
                && ((glob_pr_sglStatemachine->ul_currentAction != SGL_LINES) && (glob_pr_sglStatemachine->b_drawing_mode == SGL_MODE_LINE))) {
                glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_TRUE;
                glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
                glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;
            }
            else {
                /* Nothing to do */
            }

            if (glob_pr_sglStatemachine->b_arc_added) {
                glob_pr_sglStatemachine->b_arc_added = SGL_FALSE;
                sgl_add_arc_segment(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y, par_f_x, par_f_y);

                glob_pr_sglStatemachine->f_last_point_x = par_f_x;
                glob_pr_sglStatemachine->f_last_point_y = par_f_y;
            }
            else {
                oglxVertex2f(par_f_x, par_f_y);
            }

            glob_pr_sglStatemachine->b_new_vertex = SGL_TRUE;
        }
        else {
            /* Raise error: sglVertex2f called outside a sglBegin/sglEnd block */
            oglxSetError(SGL_ERROR_SGL_VERTEX2F, 0U);
        }
    }
    return;
}

/* End of File ***************************************************************/
