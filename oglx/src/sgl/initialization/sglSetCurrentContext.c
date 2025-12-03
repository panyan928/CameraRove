/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetCurrentContext.c
 DESCRIPTION       : sglSetCurrentContext command shall set given OGLX context as the active one
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
  NAME: sglSetCurrentContext
  DESCRIPTION:
    Function shall set given OGLX context as the active one.
  SCOPE: global
  PARAMETERS:
    par_s_context -> Context to activate
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetCurrentContext(sgl_type_statemachine * par_s_context)
{
    if (par_s_context != SGL_NULL) {
        if (glob_pr_sglStatemachine != par_s_context) {
            glob_pr_sglStatemachine = par_s_context;
            sglReset();
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_SETCURRENTCONTEXT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
