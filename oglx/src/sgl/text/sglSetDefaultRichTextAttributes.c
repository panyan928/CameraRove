/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetDefaultRichTextAttributes.c
 DESCRIPTION       : sglSetDefaultRichTextAttributes command shall set default appearance attributes of rich text
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
  NAME: sglSetDefaultRichTextAttributes
  DESCRIPTION:
    Function shall set the initial appearance attributes of rich text
  SCOPE: global
  PARAMETERS:
    par_l_foreground_color_id -> color id of foreground
    par_l_background_color_id -> color id of background
    par_l_font_id -> initial font id
    par_l_outline_value -> initial outline value
    par_b_enable -> activation of escape sequences (SGL_FALSE means they shall not be taken into account)
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetDefaultRichTextAttributes(SGLlong par_l_foreground_color_id, SGLlong par_l_background_color_id, SGLlong par_l_font_id, SGLlong par_l_outline_value,
                                     SGLbool par_b_enable)
{
    if (par_l_foreground_color_id < 0) {
        glob_pr_sglStatemachine->l_foreground_color_id = -1;
    }
    else {
        glob_pr_sglStatemachine->l_foreground_color_id = par_l_foreground_color_id;
    }

    if (par_l_background_color_id < 0) {
        glob_pr_sglStatemachine->l_background_color_id = -1;
    }
    else {
        glob_pr_sglStatemachine->l_background_color_id = par_l_background_color_id;
    }

    if (par_l_font_id < 0) {
        glob_pr_sglStatemachine->l_font_id = -1;
    }
    else {
        glob_pr_sglStatemachine->l_font_id = par_l_font_id;
    }

    if (par_l_outline_value < 0) {
        glob_pr_sglStatemachine->l_outline_value = -1;
    }
    else {
        glob_pr_sglStatemachine->l_outline_value = par_l_outline_value;
    }

    glob_pr_sglStatemachine->b_enable_esc_sequence = par_b_enable;

    return;
}

/* End of File ***************************************************************/
