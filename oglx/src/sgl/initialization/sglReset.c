/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglReset.c
 DESCRIPTION       : sglReset command shall reset the graphic machine
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

#ifdef ES2_DEV_ENV

#include "sgl_shaders.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglReset_es2
  DESCRIPTION:
    Function shall reset the specicic OpenGL ES 2.0 attributes of the graphic machine.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
SGLbool sglReset_es2(void)
{
    GLint loc_b_shader_compiled;
    GLint loc_b_linked;

    SGLbyte loc_us_return = SGL_TRUE;
    /* Initialize ES2 vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Create the fragment shader object */
    glob_pr_sglStatemachine->ui_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    /* Load the source code into it */
    glShaderSource(glob_pr_sglStatemachine->ui_frag_shader, 1, (const char **) &glob_s_fragment_shader, SGL_NULL);

    /* Compile the source code */
    glCompileShader(glob_pr_sglStatemachine->ui_frag_shader);

    /* Check if compilation succeeded */
    glGetShaderiv(glob_pr_sglStatemachine->ui_frag_shader, GL_COMPILE_STATUS, &loc_b_shader_compiled);

    if (!loc_b_shader_compiled) {
        loc_us_return = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    if (loc_us_return) {
        /* Loads the vertex shader in the same way */

        glob_pr_sglStatemachine->ui_vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(glob_pr_sglStatemachine->ui_vert_shader, 1, (const char **) &glob_s_vertex_shader, SGL_NULL);
        glCompileShader(glob_pr_sglStatemachine->ui_vert_shader);
        glGetShaderiv(glob_pr_sglStatemachine->ui_vert_shader, GL_COMPILE_STATUS, &loc_b_shader_compiled);
        if (!loc_b_shader_compiled) {
            loc_us_return = SGL_FALSE;
        }
        else {
        }

        if (loc_us_return) {
            /* Create the shader program */
            glob_pr_sglStatemachine->ui_program_object = glCreateProgram();


            /* Attach the fragment and vertex shaders to it */
            glAttachShader(glob_pr_sglStatemachine->ui_program_object, glob_pr_sglStatemachine->ui_frag_shader);
            glAttachShader(glob_pr_sglStatemachine->ui_program_object, glob_pr_sglStatemachine->ui_vert_shader);

            /* Bind the custom vertex attribute "pf_vertex_array" to location VERTEX_ARRAY */
            glBindAttribLocation(glob_pr_sglStatemachine->ui_program_object, OGLX_ES2_VERTEX_ARRAY, "pf_vertex_array");
            glBindAttribLocation(glob_pr_sglStatemachine->ui_program_object, OGLX_ES2_TEX_COORD_ARRAY, "pf_tex_coord");

            /* Link the program */
            glLinkProgram(glob_pr_sglStatemachine->ui_program_object);

            /* Check if linking succeeded in the same way we checked for compilation success */

            glGetProgramiv(glob_pr_sglStatemachine->ui_program_object, GL_LINK_STATUS, &loc_b_linked);

            if (!loc_b_linked) {
                /* Error */
                loc_us_return = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }

            if (loc_us_return) {
                /* Actually use the created program */
                glUseProgram(glob_pr_sglStatemachine->ui_program_object);

                glob_pr_sglStatemachine->ui_matrix_location = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_transformation_matrix");
                glUniformMatrix4fv(glob_pr_sglStatemachine->ui_matrix_location, 1, GL_FALSE, glob_pr_sglStatemachine->f_modelview_matrix);

                glob_pr_sglStatemachine->ui_tex_on_location = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "glob_i_texture_active");
                glob_pr_sglStatemachine->i_tex_on_state = -1;
                glUniform1i(glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "sampler2d"), 0);

                glEnable(GL_SAMPLE_COVERAGE);
                glSampleCoverage(1.0, GL_FALSE);

                glEnableVertexAttribArray(OGLX_ES2_VERTEX_ARRAY);
                glEnableVertexAttribArray(OGLX_ES2_TEX_COORD_ARRAY);
                glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;

                oglxSetTextureState(0);
                glob_pr_sglStatemachine->tf_current_color_es2[0] = 0.0F;
                glob_pr_sglStatemachine->tf_current_color_es2[1] = 0.0F;
                glob_pr_sglStatemachine->tf_current_color_es2[2] = 0.0F;
                glob_pr_sglStatemachine->tf_current_color_es2[3] = 0.0F;

                glob_pr_sglStatemachine->ui_color_location = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "v_color");
                glUniform4fv(glob_pr_sglStatemachine->ui_color_location, 1, glob_pr_sglStatemachine->tf_current_color_es2);

                oglxUpdateColor_es2(glob_pr_sglStatemachine->tf_current_color_es2);

                glob_pr_sglStatemachine->ul_framebuffer = 0UL;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    return loc_us_return;
}
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_text_attributes
  DESCRIPTION:
    Function shall reset the text attributes
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_text_attributes(void)
{
    glob_pr_sglStatemachine->ui_current_font_id = INVALID_FONT_ID;
    glob_pr_sglStatemachine->p_current_font = oglx_get_font(INVALID_FONT_ID);

    glob_pr_sglStatemachine->f_h_alignment = 0.0F;
    glob_pr_sglStatemachine->f_v_alignment = 0.0F;
    glob_pr_sglStatemachine->b_h_alignment = (SGLbyte) SGL_ALIGN_LEFT;
    glob_pr_sglStatemachine->b_v_alignment = (SGLbyte) SGL_ALIGN_BOTTOM;

    glob_pr_sglStatemachine->b_current_font_linewidth = 0U;

    glob_pr_sglStatemachine->l_current_font_linewidth_index = 0L;
    glob_pr_sglStatemachine->l_current_font_color_index = ((SGLushort) glob_pr_sglStatemachine->ul_nb_colors - 1U);
    glob_pr_sglStatemachine->ul_font_id = 0UL;
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_appearance_attributes
  DESCRIPTION:
    Function shall reset the appearance attributes
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_appearance_attributes(void)
{
    glob_pr_sglStatemachine->b_reverse_color = SGL_FALSE;

    /* Reset of global variables for OGLX line width */
    glob_pr_sglStatemachine->f_last_ogl_linewidth = 0.0F;

    /* Reset Alpha value */
    sglTransparency8((SGLlong) SGL_MAX_ALPHA_8);

    /* Reset of internal variables */
    sglIndexColori((SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1U));
    /* Index 1 is the default halo color (usually black) */
    sglIndexHaloColori(-1L);
    oglxUpdateColor((SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1U));

    glob_pr_sglStatemachine->l_second_font_color_id = 0L;
    glob_pr_sglStatemachine->l_second_font_id = (SGLlong) INVALID_FONT_ID;
    glob_pr_sglStatemachine->l_second_font_line_width = 0L;

    sglIndexLineWidthi(glob_pr_sglStatemachine->ul_nb_line_width - 1U);
    sglIndexLineStipplei(glob_pr_sglStatemachine->ul_nb_line_stipple - 1U);

    sglDisable(SGL_MODULATE);

    glob_pr_sglStatemachine->ui_ogl_texture_bound = NO_OGL_TEXTURE_BOUND;
    glob_pr_sglStatemachine->f_current_dx = 0.0F;
    glob_pr_sglStatemachine->f_current_dy = 0.0F;
    glob_pr_sglStatemachine->f_current_nx = 0.0F;
    glob_pr_sglStatemachine->f_current_ny = 0.0F;
    glob_pr_sglStatemachine->f_current_xmin = 0.0F;
    glob_pr_sglStatemachine->f_current_ymin = 0.0F;
    glob_pr_sglStatemachine->f_current_bbox_width = 1.0F;
    glob_pr_sglStatemachine->f_current_bbox_height = 1.0F;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_clipping_attributes
  DESCRIPTION:
    Function shall reset the clipping attributes
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_clipping_attributes(void)
{
    SGLulong loc_ul_i;
    SGLbyte loc_b_clipplane;

    /* Set clipplanes as undefined */
    for (loc_b_clipplane = 0U; loc_b_clipplane < SGL_MAX_CLIP_PLANES; loc_b_clipplane++) {
        glob_pr_sglStatemachine->b_clipplane_state[loc_b_clipplane] = SGL_CP_OFF;
    }

    glob_pr_sglStatemachine->tr_stencil[0].b_define = SGL_FALSE;
    glob_pr_sglStatemachine->tr_stencil[1].b_define = SGL_FALSE;
    glob_pr_sglStatemachine->tr_clipbox[0].b_define = SGL_FALSE;
    glob_pr_sglStatemachine->tr_clipbox[1].b_define = SGL_FALSE;

    glob_pr_sglStatemachine->ul_allocated_masks = 0x0;

    for (loc_ul_i = 0U; loc_ul_i < SGL_MAX_SCISSORS; loc_ul_i++) {
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].i_x = 0;
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].i_y = 0;
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].s_width = 0;
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].s_height = 0;
    }

    glob_pr_sglStatemachine->ul_number_of_scissors = 0;

#ifndef OGLX_NO_GLCLIPPLANE
    sgl_disable_clip_planes();
#endif
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_mode_attributes
  DESCRIPTION:
    Function shall reset the mode attributes
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_mode_attributes(void)
{
    glDepthFunc(GL_LEQUAL);

    glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_blend = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test = SGL_FALSE;
#ifndef ES2_DEV_ENV
    glob_pr_sglStatemachine->r_ogl_state.ul_texture_target = GL_TEXTURE_ENV;
    glob_pr_sglStatemachine->r_ogl_state.ul_texture_pname = GL_TEXTURE_ENV_MODE;
    glob_pr_sglStatemachine->r_ogl_state.l_texture_param = GL_REPLACE;
#endif
    glob_pr_sglStatemachine->f_current_depth = 0.0F;

    glDisable(GL_BLEND);
#ifndef ES2_DEV_ENV
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable(GL_SCISSOR_TEST);

    sgl_disable_stencil_test();

    /* Reset Open GL states */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Disable all SGL features */
    sglDisable(SGL_POLYGON_SMOOTH);
    sglDisable(SGL_TEXTURE_2D);
    sglDisable(SGL_HALO_SAME_LEVEL);
    sglDisable(SGL_LINE_HALOING);
    sglDisable(SGL_TESSELLATION);

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglReset
  DESCRIPTION:
    Function shall reset the graphic machine.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglReset(void)
{
    void *loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    void *loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    glob_pr_sglStatemachine->p_current_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;

    /* always use modelview matrix */
#ifndef ES2_DEV_ENV
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
#endif
    glob_pr_sglStatemachine->b_static_sequence_started = SGL_FALSE;

    /*Delete matrix stack */
    glob_pr_sglStatemachine->ul_matrix_stack_index = 0U;

    /* Loop should not be closed */
    glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
    glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
    glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;
    glob_pr_sglStatemachine->tr_geometry.l_size = 0;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices = 0;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0;

    sgl_reset_appearance_attributes();

    /* It is set to glob_pr_sglStatemachine->ul_number_of_textures, it means that no texture is bound */
    glob_pr_sglStatemachine->l_last_texture_bound_index = (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures;

    /* No vertex given */
    glob_pr_sglStatemachine->b_new_vertex = SGL_FALSE;

    /*Set modelview matrix to the ortho values */
    sglLoadIdentity();

    /* Reset all clipping attributes */
    sgl_reset_clipping_attributes();

    /* Reset all text attributes */
    sgl_reset_text_attributes();

    /* Reset all mode attributes */
    sgl_reset_mode_attributes();

    glob_ul_triangle_number = 0UL;

#ifndef ES2_DEV_ENV
    glAlphaFunc(GL_GREATER, 0.5F);

    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
#ifndef OGLX_NO_VERTEX_ARRAY
    /* Enable Vertex arrays */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_vertex_array);
#endif

    /* Reset texture environment mode */
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
#else
    glVertexAttribPointer(OGLX_ES2_TEX_COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexAttribPointer(OGLX_ES2_VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_vertex_array);
#endif
    glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;

    return;
}

/* End of File ***************************************************************/
