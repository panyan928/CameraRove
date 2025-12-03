/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEndScissor.c
 DESCRIPTION       : sglEndScissor command shall restore previous scissor or disable scissor test
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
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglEndScissor
  DESCRIPTION:
    Function shall restore previous scissor or disable scissor test.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglEndScissor(void)
{
    if (glob_pr_sglStatemachine->ul_number_of_scissors > 0U) {
        glob_pr_sglStatemachine->ul_number_of_scissors--;

        if (glob_pr_sglStatemachine->ul_number_of_scissors > 0U) {
            SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_number_of_scissors - 1U;

            /* Activate last scissor on stack */
            glScissor(glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].i_x, glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].i_y,
                      glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].s_width, glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].s_height);
        }
        else {
            /* Disable scissor test if there is no more scissor on stack */
            oglxDisable(GL_SCISSOR_TEST);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_ENDSCISSOR, 0);
    }
}
