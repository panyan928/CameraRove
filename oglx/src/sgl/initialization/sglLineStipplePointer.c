/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglLineStipplePointer.c
 DESCRIPTION       : sglLineStipplePointer command shall define the linestipple 
					map to be used
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
#pragma attol crc_justify_all (Rule M17.4, 2000, "Rule M17.4 is not applied here as we do not know by advance the dimensions of the input line stipple.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


const sgl_linestipple default_line_stipple_table[DEFAULT_LINE_STIPPLE_NUMBER] = {
    {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, 1, 0},
    {{12, 12, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, 0, 2},
    {{8, 8, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, 0, 2},
    {{1, 6, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, 0, 2},
    {{1, 8, 8, 8, 0, 0}, {1, 0, 1, 0, 0, 0}, 0, 4}
};

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglLineStipplePointer
  DESCRIPTION:
    Function shall define the linestipple map to be used.
  SCOPE: global
  PARAMETERS:  
    par_ps_linestipple -> Pointer to the linestipple map  
	par_ul_nb_line_stipple -> Number of entries in par_ps_linestipple
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglLineStipplePointer(const sgl_linestipple * par_ps_linestipple, SGLulong par_ul_nb_line_stipple)
{

    if ((par_ps_linestipple != SGL_NULL) && (par_ul_nb_line_stipple > 0)) {
        SGLbyte loc_b_error_detected = 0U;

        /* Check function is not called between sglBegin/sglEnd */
        if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
            loc_b_error_detected = 1U;
        }
        else {
            SGLulong loc_ul_i;
            for (loc_ul_i = 0UL; loc_ul_i < par_ul_nb_line_stipple; loc_ul_i++) {
                /* Robustness: check the number of segments is not greater than the number of entries in the table */
                if (par_ps_linestipple[loc_ul_i].b_number > SGL_MAX_STIPPLES_LENGTH) {
                    loc_b_error_detected = 1U;
                }
            }
        }

        if (!loc_b_error_detected) {
            glob_pr_sglStatemachine->tr_linestipple = (sgl_linestipple *) par_ps_linestipple;
            glob_pr_sglStatemachine->ul_nb_line_stipple = par_ul_nb_line_stipple;
        }
        else {
            /* Error if function is called between sglBegin/sglEnd */
            oglxSetError(SGL_ERROR_SGL_LINESTIPPLEPOINTER, 0U);
        }
    }
    else {
        /* Error if function is called between sglBegin/sglEnd */
        oglxSetError(SGL_ERROR_SGL_LINESTIPPLEPOINTER, 0U);
    }

    return;
}

/* End of File ***************************************************************/
