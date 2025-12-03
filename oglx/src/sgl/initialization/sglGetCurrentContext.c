/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetCurrentContext.c
 DESCRIPTION       : sglGetCurrentContext shall return OGLX current context
 VERSION           : $Revision$
 MODIFICATION DATE : $Date$
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
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
  NAME: sglGetCurrentContext
  DESCRIPTION:
    Function shall return OGLX current context
  SCOPE: global
  PARAMETERS:
	None
  RETURN:
    sgl_type_statemachine *-> OGLX current context
---------------------------------------------------------------------+*/
sgl_type_statemachine *sglGetCurrentContext(void)
{
    return glob_pr_sglStatemachine;
}
