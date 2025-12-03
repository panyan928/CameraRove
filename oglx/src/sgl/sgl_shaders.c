/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_shaders.c
 DESCRIPTION       : shaders definition
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/
#include "sgl.h"

#ifdef ES2_DEV_ENV

/* Fragment shader */
char *glob_s_fragment_shader = "\
uniform sampler2D sampler2d;\
uniform highp vec4 v_color;\
varying highp vec2	v_tex_coord;\
uniform int glob_i_texture_active;\
void main (void)\
{\
	if(glob_i_texture_active < 1)\
	{\
		gl_FragColor = v_color;\
	}\
	else\
	{\
		if(glob_i_texture_active < 3)\
		{\
			gl_FragColor = texture2D(sampler2d,v_tex_coord);\
		}\
		else\
		{\
			if(glob_i_texture_active < 5)\
			{\
				highp vec4 loc_texture_color = texture2D(sampler2d,v_tex_coord);\
				gl_FragColor =  vec4(v_color[0], v_color[1], v_color[2], loc_texture_color[3]);\
			}\
			else\
			{\
				gl_FragColor = texture2D(sampler2d,v_tex_coord) * v_color;\
			}\
		}\
	}\
}";

/* Vertex shader */
char *glob_s_vertex_shader = "\
attribute highp vec4 pf_vertex_array;\
attribute highp vec2 pf_tex_coord;\
uniform highp vec4 v_color;\
uniform highp mat4	pf_transformation_matrix;\
uniform int glob_i_texture_active;\
varying highp vec2	v_tex_coord;\
void main(void)\
{\
	v_tex_coord = pf_tex_coord;\
	gl_Position = pf_transformation_matrix * pf_vertex_array;\
}";


#endif

/* End of File ***************************************************************/
