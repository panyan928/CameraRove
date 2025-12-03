/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetWordsSeparators.c
 DESCRIPTION       : sglSetWordsSeparators command shall fill the words separators table
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
#pragma attol crc_justify_all (Rule M16.7, 2000, "Rule M16.7 is not applied here as the prototype of the function is specified like this.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetWordsSeparators
  DESCRIPTION:
    Function shall fill the words separators table.
  SCOPE: global
  PARAMETERS:
    par_pb_separators -> table containing separators
    par_ul_nb_separators -> number of separators
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetWordsSeparators(SGLbyte * par_pb_separators, SGLulong par_ul_nb_separators)
{
    if ((par_ul_nb_separators <= SGL_MAX_SEPARATORS) && (par_pb_separators != SGL_NULL)) {
        SGLulong loc_ul_i;
        for (loc_ul_i = 0; loc_ul_i < par_ul_nb_separators; loc_ul_i++) {
            glob_pr_sglStatemachine->tb_word_separators[loc_ul_i] = par_pb_separators[loc_ul_i];
        }
        glob_pr_sglStatemachine->ul_nb_word_separators = par_ul_nb_separators;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_SETWORDSSEPARATORS, (SGLulong) par_ul_nb_separators);
    }
}

/* End of File ***************************************************************/
