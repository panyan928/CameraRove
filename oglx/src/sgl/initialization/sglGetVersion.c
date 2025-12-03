/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetVersion.c
 DESCRIPTION       : sglGetVersion shall return OGLX version
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"
#include "oglx_version.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M6.3, 2000, "Rule M6.3 is not applied here as the function returns a C string and OGLX does not provide this kind of type.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetVersion
  DESCRIPTION:
    Function shall return OGLX version
  SCOPE: global
  PARAMETERS:
	None
  RETURN:
    SGLbyte *-> OGLX version
---------------------------------------------------------------------+*/
char *sglGetVersion(void)
{
    return OGLX_VERSION;
}
