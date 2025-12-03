/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetSecondFontAttributes.c
 DESCRIPTION       : sglSetSecondFontAttributes command shall set appearance attributes of second font
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
  NAME: sglSetSecondFontAttributes
  DESCRIPTION:
    Function shall set appearance attributes of second font
  SCOPE: global
  PARAMETERS:
    par_l_second_font_color_id -> color id of second font
    par_l_second_font_id -> second font index
    par_l_second_font_line_width -> line width of second font
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetSecondFontAttributes(SGLlong par_l_second_font_color_id, SGLlong par_l_second_font_id, SGLlong par_l_second_font_line_width)
{
    glob_pr_sglStatemachine->l_second_font_color_id = par_l_second_font_color_id;
    glob_pr_sglStatemachine->l_second_font_id = par_l_second_font_id;
    glob_pr_sglStatemachine->l_second_font_line_width = par_l_second_font_line_width;

    return;
}

/* End of File ***************************************************************/
