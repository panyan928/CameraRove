/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxSelectFont.c
 DESCRIPTION       : Select font for the function sgluWriteText
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

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxSelectFont
  DESCRIPTION:
    Function shall select font associated texture for a textured font and 
    default font if the given font id is greater than glob_pr_sglStatemachine->ul_nb_of_fonts.
  SCOPE: global
  PARAMETERS:
    par_ui_font_identifier -> font identifier, range: [0,SGL_MAX_NUMBER_OF_FONTS-1]
  RETURN:			
    void
---------------------------------------------------------------------+*/
void oglxSelectFont(SGLulong par_ui_font_identifier)
{
    /* Store font pointer for performance */
    glob_pr_sglStatemachine->p_current_font = oglx_get_font(par_ui_font_identifier);

    /* Check if given font id is in the valid range of values */
    if (par_ui_font_identifier >= glob_pr_sglStatemachine->ul_nb_of_fonts) {
        glob_pr_sglStatemachine->ui_current_font_id = INVALID_FONT_ID;
    }
    else {
        glob_pr_sglStatemachine->ui_current_font_id = par_ui_font_identifier;

        if (glob_pr_sglStatemachine->p_current_font->us_font_kind == GLF_FONT_TYPE_BITMAP) {
            /*It is necessary to restore the blending and texture states, because 
               when a line is drawn they are being disabled. */
#ifndef ES2_DEV_ENV
            oglxEnable(GL_TEXTURE_2D);
#endif
            oglxEnable(GL_BLEND);
        }
        else {
            /* Nothing to do */
        }
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxSelectFontTexture
  DESCRIPTION:
    Function shall select texture given as parameter.
    It corresponds to a specific font line width.
  SCOPE: global
  PARAMETERS:		
    par_ui_texture_number -> font texture number, range: [0,GLF_TEXTURE_MAP_NUMBER]
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglxSelectFontTexture(SGLulong par_ui_texture_number)
{

    /* Check if given texture id is in valid range */
    if (par_ui_texture_number >= GLF_MAX_NUMBER_OF_TEXTURE) {
        par_ui_texture_number = 0U;
    }
    else {
        /* Nothing to do */
    }

    /* Bind texture */
    oglxBindTexture(glob_pr_sglStatemachine->tui_texture_id[par_ui_texture_number]);

    return;
}

/* End of File ***************************************************************/
