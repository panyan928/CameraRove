/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetTextureInternalFormat.c
 DESCRIPTION       : sglSetTextureInternalFormat command shall select the internal 
					texture mode to store texture with transparency and without 
					transparency in the OpenGL driver
 *  COPYRIGHT (C)     : 2010 by Esterel Technologies. All Rights Reserved.
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
  NAME: sglSetTextureInternalFormat
  DESCRIPTION:
    Function shall select the internal texture mode to 
    store texture with transparency and without transparency 
    in the OpenGL driver.
  SCOPE: global
  PARAMETERS:		
    par_l_tex_format_without_alpha -> Format of textures without transparency
    (GL_ALPHA or GL_RGB or GL_RGBA or GL_LUMINANCE or GL_LUMINANCE_ALPHA
    or GL_COLOR_INDEX)
    par_l_tex_format_with_alpha -> Format of textures with transparency
    (GL_ALPHA or GL_RGB or GL_RGBA or GL_LUMINANCE or GL_LUMINANCE_ALPHA
    or GL_COLOR_INDEX)            
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetTextureInternalFormat(SGLlong par_l_tex_format_without_alpha, SGLlong par_l_tex_format_with_alpha)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        /* Store texture internal format in OGLX context */
        glob_pr_sglStatemachine->i_texture_internal_format = (GLint) par_l_tex_format_without_alpha;
        glob_pr_sglStatemachine->i_alpha_texture_internal_format = (GLint) par_l_tex_format_with_alpha;
    }
    else {
        /* Error if function is called between sglBegin/sglEnd */
        oglxSetError(SGL_ERROR_SGL_SETTEXTUREINTERNALFORMAT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
