/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetErrors.c
 DESCRIPTION       : sglGetErrors command shall fill the error buffer given as 
					parameter
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
  NAME: sglGetErrors
  DESCRIPTION:
    Function shall fill the error buffer given as parameter.
  SCOPE: global
  PARAMETERS:	
    par_pul_error_buffer -> Address to store the array of errors
    par_pul_error_number -> Address to store the number of errors to read
  RETURN:			
    SGLbyte -> state of the returned error stack 
    SGL_NO_ERROR: empty error stack
    SGL_ERROR_STACK_UNDERFLOW: not a full error stack
    SGL_ERROR_STACK_OVERFLOW: a full error stack 
---------------------------------------------------------------------+*/
SGLbyte sglGetErrors(SGLulong * par_pul_error_buffer, SGLulong * par_pul_error_number)
{
    SGLbyte loc_b_return_value = SGL_NO_ERROR;
    SGLulong loc_ul_index;
    SGLbyte loc_b_parameters_ok = 1U;

    /* Check error buffer is not NULL */
    if (par_pul_error_buffer == SGL_NULL) {
        loc_b_parameters_ok = 0U;
    }
    else {
        /* Nothing to do */
    }

    /* Check number buffer is not NULL */
    if (par_pul_error_number == SGL_NULL) {
        loc_b_parameters_ok = 0U;
    }
    else {
        /* Nothing to do */
    }

    if (loc_b_parameters_ok != 0U) {
        if (glob_pr_sglStatemachine->ul_error_counter == 0U) {
            /* If there is no error it shall be stated by return value of the function (set to SGL_NO_ERROR) */
            *par_pul_error_number = 0UL;

            for (loc_ul_index = 0UL; loc_ul_index < SGL_ERROR_MAX; loc_ul_index++) {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M17.4, 5, "Rule M17.4 is not applied here as par_pul_error_buffer size shall be SGL_ERROR_MAX*2 but is not declared as a table.")
#endif
                *par_pul_error_buffer = 0UL;
                par_pul_error_buffer++;
                *par_pul_error_buffer = 0UL;
                par_pul_error_buffer++;
            }
            /* Not necessary to delete the error table */
        }
        else {
            if (!(glob_pr_sglStatemachine->b_overflow_flag)) {
                /* Fill the given table with stored errors and set return value to SGL_ERROR_STACK_UNDERFLOW */
                loc_b_return_value = SGL_ERROR_STACK_UNDERFLOW;
                *par_pul_error_number = (SGLulong) glob_pr_sglStatemachine->ul_error_counter;

                for (loc_ul_index = 0UL; loc_ul_index < glob_pr_sglStatemachine->ul_error_counter; loc_ul_index++) {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M17.4, 5, "Rule M17.4 is not applied here as par_pul_error_buffer size shall be SGL_ERROR_MAX*2 but is not declared as a table.")
#endif
                    *par_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][0];
                    par_pul_error_buffer++;
                    *par_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][1];
                    par_pul_error_buffer++;
                }

            }
            else {
                /* Fill the given table with stored errors and set return value to SGL_ERROR_STACK_OVERFLOW */
                loc_b_return_value = SGL_ERROR_STACK_OVERFLOW;
                *par_pul_error_number = (SGLulong) SGL_ERROR_MAX;

                for (loc_ul_index = 0UL; loc_ul_index < SGL_ERROR_MAX; loc_ul_index++) {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M17.4, 5, "Rule M17.4 is not applied here as par_pul_error_buffer size shall be SGL_ERROR_MAX*2 but is not declared as a table.")
#endif
                    *par_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][0];
                    par_pul_error_buffer++;
                    *par_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][1];
                    par_pul_error_buffer++;
                }
            }
        }
    }
    else {
        /* Nothing to do */
    }
    return (loc_b_return_value);

}

/* End of File ***************************************************************/
