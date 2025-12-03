/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEnd.c
 DESCRIPTION       : sglEnd shall end the vertices 
					of a primitive or a group of primitives.
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Project specific includes
 *****************************************************************************/

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
#endif

sgl_triangle glob_t_triangles[SGL_MAX_VERTEX_ARRAY_SIZE];
sgl_triangle glob_t_texture_triangles[SGL_MAX_VERTEX_ARRAY_SIZE];
SGLulong glob_ul_triangle_number;



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_stencil
  DESCRIPTION:
    Function shall draw the stencil.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_stencil(void)
{
    void *loc_p_vertex_array;

    /* Call the vertex function to give the stencil definition to OpenGL driver */
    loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    if (!glob_pr_sglStatemachine->b_tessellation) {
#ifndef ES2_DEV_ENV
        oglxDrawVertexArray(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#else
        oglxDrawVertexArray_es2(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#endif
    }
    else {
        sgl_tessellation();
    }
    glob_pr_sglStatemachine->tr_geometry.l_size = 0;

    /* allow drawing to color */
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glStencilMask(0x0U);

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_close_line_loop
  DESCRIPTION:
    Function shall close a line loop if necessary.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_close_line_loop(void)
{
    /* Only finish the draw in case of a line loop */
    if (glob_pr_sglStatemachine->b_close_line_loop) {
        if (glob_pr_sglStatemachine->b_arc_added) {
            glob_pr_sglStatemachine->b_arc_added = SGL_FALSE;
            sgl_add_arc_segment(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y,
                                glob_pr_sglStatemachine->f_line_loop_first_vertex_x, glob_pr_sglStatemachine->f_line_loop_first_vertex_y);
            glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
        }
        else {
            glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
            /* Add the ending point in case of a line stipple */
            if (glob_pr_sglStatemachine->b_stipple_ok) {
                oglxVertex2f(glob_pr_sglStatemachine->f_line_loop_first_vertex_x, glob_pr_sglStatemachine->f_line_loop_first_vertex_y);
            }
            else {
                /* Nothing to do */
            }
        }

    }
    else {
        /* Nothing to do */
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_finish_shape_arc
  DESCRIPTION:
    Function shall finish the definition of a shape if it is ended by an arc
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_finish_shape_arc(void)
{
    if (glob_pr_sglStatemachine->b_arc_added) {
        glob_pr_sglStatemachine->b_arc_added = SGL_FALSE;
        sgl_add_arc_segment(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y,
                            glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0], glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1]);
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_point_in_polygon
  DESCRIPTION:
    Function shall state if input point is inside given polygon (ray casting algorithm is used)
  SCOPE: global
  PARAMETERS:
    par_ul_nb_vertices -> Number of vertices of the polygon
	par_p_vertices -> vertices of the polygon
	par_f_x -> x coordinate of the input point
	par_f_y -> x coordinate of the input point
  RETURN:
    SGLbool -> SGL_TRUE if input point is inside given polygon, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_point_in_polygon(SGLulong par_ul_nb_vertices, sgl_vector2d * par_p_vertices, SGLfloat par_f_x, SGLfloat par_f_y)
{
    SGLulong i;
    SGLulong j = par_ul_nb_vertices - 1;
    SGLbool loc_b_return = SGL_FALSE;

    for (i = 0; i < par_ul_nb_vertices; i++) {
        if ((((par_p_vertices[i][1] < par_f_y) && (par_p_vertices[j][1] >= par_f_y)) || ((par_p_vertices[j][1] < par_f_y) && (par_p_vertices[i][1] >= par_f_y)))
            && ((par_p_vertices[i][0] <= par_f_x) || (par_p_vertices[j][0] <= par_f_x))) {
            if (par_p_vertices[i][0] +
                SGLfloat_div((par_f_y - par_p_vertices[i][1]),
                             (par_p_vertices[j][1] - par_p_vertices[i][1])) * (par_p_vertices[j][0] - par_p_vertices[i][0]) < par_f_x) {
                loc_b_return = !loc_b_return;
            }
        }
        j = i;
    }

    return loc_b_return;

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_ear
  DESCRIPTION:
    Function shall state if the point at given index is an ear for the ear clipping algorithm
  SCOPE: global
  PARAMETERS:
    par_ul_ear_index -> index of point to check
  RETURN:
    SGLbool -> SGL_TRUE if input point is an ear, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_is_ear(SGLulong par_ul_ear_index)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLulong loc_ul_end;
    SGLulong loc_ul_i;
    SGLulong loc_ul_ear_index = par_ul_ear_index;
    SGLbool loc_b_ear = SGL_TRUE;
    sgl_point loc_pt, loc_s_v1, loc_s_v2, loc_s_v3;

    if (par_ul_ear_index == 0UL) {
        loc_s_v1.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1][0];
        loc_s_v1.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1][1];
        loc_ul_i = 2UL;
        loc_ul_end = (SGLulong) (loc_l_size - 1);
    }
    else {
        loc_ul_i = 0UL;
        loc_ul_end = (SGLulong) loc_l_size;
        loc_s_v1.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1][0];
        loc_s_v1.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1][1];
    }
    loc_s_v2.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][0];
    loc_s_v2.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][1];
    loc_s_v3.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1][0];
    loc_s_v3.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1][1];

    /* No other point of the polygon shall be in the triangle defined by the ear candiate and the preceding and following point */
    while ((loc_ul_i < loc_ul_end) && loc_b_ear) {
        if (loc_ul_i != (loc_ul_ear_index - 1)) {
            loc_pt.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][0];
            loc_pt.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][1];

            if (mth_point_in_triangle(&loc_pt, &loc_s_v1, &loc_s_v2, &loc_s_v3)) {
                loc_b_ear = SGL_FALSE;
            }
            else {
                loc_ul_i++;
            }
        }
        else {
            loc_ul_i += 3;
        }
    }

    /* Only a convex vertex can be an ear */
    if (loc_b_ear) {
        SGLfloat loc_f_x = (loc_s_v1.f_x + loc_s_v3.f_x) / 2.0F;
        SGLfloat loc_f_y = (loc_s_v1.f_y + loc_s_v3.f_y) / 2.0F;

        /* The point is convex if the center of the segment between the two other points of the triangle are inside the polygon */
        if (!sgl_point_in_polygon(loc_l_size, glob_pr_sglStatemachine->tr_geometry.p_vertices, loc_f_x, loc_f_y)) {
            loc_b_ear = SGL_FALSE;
        }
    }

    return loc_b_ear;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_get_first_ear
  DESCRIPTION:
    Function shall return the first ear of current polygon
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    SGLulong -> Index of first detected ear
---------------------------------------------------------------------+*/
SGLulong sgl_get_first_ear(void)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLulong loc_ul_ear_index = 0UL;
    SGLbool loc_b_ear_found = SGL_FALSE;

    while ((loc_ul_ear_index < (SGLulong) loc_l_size) && !loc_b_ear_found) {
        loc_b_ear_found = sgl_is_ear(loc_ul_ear_index);

        if (!loc_b_ear_found) {
            loc_ul_ear_index++;
        }
    }

    if (loc_ul_ear_index == (SGLulong) loc_l_size) {
        loc_ul_ear_index = 0UL;
    }

    return loc_ul_ear_index;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_add_triangle_suppress_ear
  DESCRIPTION:
    Function shall add triangle with edge at index par_ul_ear_index to triangle stack and suppress it from current polygon
  SCOPE: global
  PARAMETERS:
    par_ul_ear_index -> Index of the detected ear
	par_b_texture -> Texture active or not
  RETURN:
    SGLulong -> Size of the remaining polygon
---------------------------------------------------------------------+*/
SGLulong sgl_add_triangle_suppress_ear(SGLulong par_ul_ear_index, SGLbool par_b_texture)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

    if (glob_ul_triangle_number < SGL_MAX_VERTEX_ARRAY_SIZE) {
        SGLulong loc_ul_i;

        if (par_ul_ear_index == 0UL) {
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1][0];
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1][1];
            if (par_b_texture) {
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_l_size - 1][0];
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_l_size - 1][1];
            }
        }
        else {
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1][0];
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1][1];
            if (par_b_texture) {
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index - 1][0];
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index - 1][1];
            }
        }

        glob_t_triangles[glob_ul_triangle_number].s_p2.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][0];
        glob_t_triangles[glob_ul_triangle_number].s_p2.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][1];
        glob_t_triangles[glob_ul_triangle_number].s_p3.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1][0];
        glob_t_triangles[glob_ul_triangle_number].s_p3.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1][1];

        if (par_b_texture) {
            glob_t_texture_triangles[glob_ul_triangle_number].s_p2.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index][0];
            glob_t_texture_triangles[glob_ul_triangle_number].s_p2.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index][1];
            glob_t_texture_triangles[glob_ul_triangle_number].s_p3.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index + 1][0];
            glob_t_texture_triangles[glob_ul_triangle_number].s_p3.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index + 1][1];
        }
        glob_ul_triangle_number++;

        for (loc_ul_i = par_ul_ear_index; loc_ul_i < (SGLulong) (loc_l_size - 1); loc_ul_i++) {
            glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i + 1][0];
            glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i + 1][1];
            if (par_b_texture) {
                glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i][0] = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i + 1][0];
                glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i][1] = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i + 1][1];
            }
        }

        glob_pr_sglStatemachine->tr_geometry.l_size = loc_l_size - 1;
        glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = loc_l_size - 1;
    }

    return glob_pr_sglStatemachine->tr_geometry.l_size;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_triangles
  DESCRIPTION:
    Function shall draw the triangles of the triangle stack
  SCOPE: global
  PARAMETERS:
	par_b_texture -> Texture active or not
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_triangles(SGLbool par_b_texture)
{
    sgl_vector2d loc_t_vertex_array[3];
    sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    SGLulong loc_ul_i;
    for (loc_ul_i = 0UL; loc_ul_i < glob_ul_triangle_number; loc_ul_i++) {
        sgl_triangle loc_s_triangle = glob_t_triangles[loc_ul_i];
        loc_t_vertex_array[0][0] = loc_s_triangle.s_p1.f_x;
        loc_t_vertex_array[0][1] = loc_s_triangle.s_p1.f_y;
        loc_t_vertex_array[1][0] = loc_s_triangle.s_p2.f_x;
        loc_t_vertex_array[1][1] = loc_s_triangle.s_p2.f_y;
        loc_t_vertex_array[2][0] = loc_s_triangle.s_p3.f_x;
        loc_t_vertex_array[2][1] = loc_s_triangle.s_p3.f_y;
        if (par_b_texture) {
            sgl_triangle loc_s_texture_triangle = glob_t_texture_triangles[loc_ul_i];
            loc_t_tex_coord_array[0][0] = loc_s_texture_triangle.s_p1.f_x;
            loc_t_tex_coord_array[0][1] = loc_s_texture_triangle.s_p1.f_y;
            loc_t_tex_coord_array[1][0] = loc_s_texture_triangle.s_p2.f_x;
            loc_t_tex_coord_array[1][1] = loc_s_texture_triangle.s_p2.f_y;
            loc_t_tex_coord_array[2][0] = loc_s_texture_triangle.s_p3.f_x;
            loc_t_tex_coord_array[2][1] = loc_s_texture_triangle.s_p3.f_y;
#ifndef ES2_DEV_ENV
            oglxDrawTexVertexArray(GL_TRIANGLE_FAN, 3, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
#else
            if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
                if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                    oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, 3, &(loc_t_vertex_array[0][0]), SGL_TRUE);
                }
                else {
                    oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, 3, &(loc_t_vertex_array[0][0]), SGL_FALSE);
                }
            }
#endif

        }
        else {
#ifndef ES2_DEV_ENV
            oglxDrawVertexArray(GL_TRIANGLE_FAN, 3, &(loc_t_vertex_array[0][0]));
#else
            oglxDrawVertexArray_es2(GL_TRIANGLE_FAN, 3, &(loc_t_vertex_array[0][0]));
#endif
        }
    }

    /* Reset triangles number */
    glob_ul_triangle_number = 0UL;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_tessellation
  DESCRIPTION:
    Function shall perform a tessellation of current polygon with ear clipping algorithm
  SCOPE: global
  PARAMETERS:
	None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_tessellation(void)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLbool loc_b_texture = (glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord > 0L);

    while (loc_l_size > 3) {
        SGLulong loc_ul_ear_index = sgl_get_first_ear();
        loc_l_size = sgl_add_triangle_suppress_ear(loc_ul_ear_index, loc_b_texture);
    }

    /* Add the last triangle */
    (void) sgl_add_triangle_suppress_ear(1UL, loc_b_texture);

    sgl_draw_triangles(loc_b_texture);

    glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_shape
  DESCRIPTION:
    Function shall draw the defined shape
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_shape(void)
{
    SGLbyte loc_b_close_line = 1U;
    SGLbool loc_b_complex_shape = (glob_pr_sglStatemachine->b_draw_crown || glob_pr_sglStatemachine->b_draw_ellipse || glob_pr_sglStatemachine->b_draw_circle);

    if ((!glob_pr_sglStatemachine->b_tessellation) || loc_b_complex_shape) {
        void *loc_p_vertex_array;
#ifndef ES2_DEV_ENV
        void *loc_p_tex_vertex_array;
#endif
        /* Call the vertex function to give vertices an textures to OpenGL driver */
        if (glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord == 0L) {
            if (glob_pr_sglStatemachine->ul_currentAction == SGL_POLYGON) {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#ifndef ES2_DEV_ENV
                oglxDrawVertexArray(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#else
                oglxDrawVertexArray_es2(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#endif
            }
            else {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#ifndef ES2_DEV_ENV
                oglxDrawVertexArray(GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#else
                oglxDrawVertexArray_es2(GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array);
#endif
            }
        }
        else {
#ifdef ES2_DEV_ENV
            SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
#endif
            if (glob_pr_sglStatemachine->ul_currentAction == SGL_POLYGON) {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#ifndef ES2_DEV_ENV
                loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
                oglxDrawTexVertexArray(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size,
                                       (const GLfloat *) loc_p_vertex_array, (const GLfloat *) loc_p_tex_vertex_array);
#else
                if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                    if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                        oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array,
                                                   SGL_TRUE);
                    }
                    else {
                        oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array,
                                                   SGL_FALSE);
                    }
                }
#endif
                glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
            }
            else {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#ifndef ES2_DEV_ENV
                loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
                oglxDrawTexVertexArray(GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size,
                                       (const GLfloat *) loc_p_vertex_array, (const GLfloat *) loc_p_tex_vertex_array);
#else
                if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                    if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                        oglxDrawTexVertexArray_es2(GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array,
                                                   SGL_TRUE);
                    }
                    else {
                        oglxDrawTexVertexArray_es2(GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const GLfloat *) loc_p_vertex_array,
                                                   SGL_FALSE);
                    }
                }
#endif
                glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
            }
        }

        if (glob_pr_sglStatemachine->b_polygon_smooth != SGL_TRUE) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->b_transparency != SGL_MAX_ALPHA_8) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->b_texture_state) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        /* Draw a closed line for polygon smooth when no transparency and no texture */
        if (loc_b_close_line != 0U) {
            sgl_polygon_smooth_backup loc_struct_backup;

            sgl_backup_for_polygon_smooth(&loc_struct_backup);

            sgl_draw_line();

            sgl_restore_after_polygon_smooth(&loc_struct_backup);

        }
        else {
            /* Nothing to do */
        }

    }
    else {
        if (glob_pr_sglStatemachine->b_polygon_smooth != SGL_TRUE) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->b_transparency != SGL_MAX_ALPHA_8) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->b_texture_state) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        /* Draw a closed line for polygon smooth when no transparency and no texture */
        if (loc_b_close_line != 0U) {
            sgl_polygon_smooth_backup loc_struct_backup;

            sgl_backup_for_polygon_smooth(&loc_struct_backup);

            sgl_draw_line();

            sgl_restore_after_polygon_smooth(&loc_struct_backup);

        }
        else {
            /* Nothing to do */
        }
        sgl_tessellation();
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglEnd
  DESCRIPTION:
    Function shall terminate a primitive description.
  SCOPE: global
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEnd(void)
{
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;

    if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
        if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
            loc_b_texture_alpha = SGL_TRUE;
        }
    }

    if (glob_pr_sglStatemachine->tr_geometry.l_size > 1) {
        switch (glob_pr_sglStatemachine->ul_currentAction) {
        case SGL_LINES:
        {
            sgl_end_line();
            glob_pr_sglStatemachine->b_sgl_lines = SGL_FALSE;
        }
            break;
        case SGL_LINE_STRIP:
        case SGL_LINE_LOOP:
        {
            sgl_end_line();
        }
            break;
        case SGL_TRIANGLE_STRIP:
        case SGL_POLYGON:
        {
            /* Nothing shall be drawn if a texture of type SGL_BITMAP_ALPHA is defined and if modulate is inactive */
            if ((glob_pr_sglStatemachine->b_texture_state) && loc_b_texture_alpha && !glob_pr_sglStatemachine->b_modulate) {
                /* Nothing to do */
            }
            else {
                sgl_end_shape();
                /* Restore Depth test if necessary */
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxEnable(GL_DEPTH_TEST);
                }
            }
        }
            break;
        case SGL_STENCIL1:
        {
            sgl_stencil_buffer_Stencil_end();
            glob_pr_sglStatemachine->tr_stencil[0].b_define = SGL_TRUE;
        }
            break;
        case SGL_STENCIL2:
        {
            sgl_stencil_buffer_Stencil_end();
            glob_pr_sglStatemachine->tr_stencil[1].b_define = SGL_TRUE;
        }
            break;
        default:
        {
            /* Nothing to do */
        }
            break;
        }
    }
    else {
        /* Nothing to do */
    }

    glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;

    /* reset the switch of line mode */
    glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_FALSE;
    glob_pr_sglStatemachine->f_last_stipple_length = 0.0F;
    glob_pr_sglStatemachine->ul_last_stipple_index = 0UL;
    glob_pr_sglStatemachine->b_odd_vertex = SGL_FALSE;

    return;
}

/* End of File ***************************************************************/
