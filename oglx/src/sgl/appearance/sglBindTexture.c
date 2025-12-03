/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBindTexture.c
 DESCRIPTION       : sglBindTexture shall set the specified texture as active
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
  NAME: sglBindTexture
  DESCRIPTION:
    Function shall set the specified texture as active.
  SCOPE: global
  PARAMETERS:
    par_b_target -> Texture type, equal to SGL_TEXTURE_2D, any other value is an error.
    par_l_texture_number -> Number of the active texture, range: [0,glob_pr_sglStatemachine->ul_number_of_textures[
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglBindTexture(SGLbyte par_b_target, SGLlong par_l_texture_number)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check target is the good one */
    if (par_b_target != SGL_TEXTURE_2D) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check Function is not called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture attribute buffer is not NULL */
    if (glob_pr_sglStatemachine->p_texture_attrib == SGL_NULL) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        /* Check texture number is valid */
        if ((par_l_texture_number >= (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) || (par_l_texture_number < 0L)) {
            loc_b_error_detected = 1U;
        }
        else {
            /* Nothing to do */
        }

        if (!loc_b_error_detected) {
            /* Check given texture id is not already loaded */
            if (!(glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_specified_state)) {
                oglxSetError(SGL_ERROR_SGL_BINDTEXTURE, (SGLulong) par_l_texture_number);
            }
            else {
                /* Set texture number as active one */
                glob_pr_sglStatemachine->l_last_texture_bound_index = par_l_texture_number;
            }
        }
        else {
            oglxSetError(SGL_ERROR_SGL_BINDTEXTURE, (SGLulong) par_l_texture_number);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_BINDTEXTURE, (SGLulong) par_b_target);
    }

    return;
}

/* End of File ***************************************************************/
