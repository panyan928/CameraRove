/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetCharsetEncoding.c
 DESCRIPTION       : sglSetCharsetEncoding command shall define the characters encoding format
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/
/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetCharsetEncoding
  DESCRIPTION:
    Function shall define the characters encoding format.
  SCOPE: global
  PARAMETERS:
    par_b_format -> encoding format
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetCharsetEncoding(SGLbyte par_b_format)
{
    if ((par_b_format != SGL_FORMAT_ASCII) && (par_b_format != SGL_FORMAT_UTF8)
        && (par_b_format != SGL_FORMAT_GBK)) {
        oglxSetError(SGL_ERROR_SGL_SETCHARSETENCODING, (SGLulong) par_b_format);
    }
    else {
        glob_pr_sglStatemachine->b_charset_encoding = par_b_format;
    }
}

/* End of File ***************************************************************/
