/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxColor4fv.c
 DESCRIPTION       : Call to glColor4f if color changes using table of colors
 COPYRIGHT (C)     : 2010 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/
/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"
#include "mth.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:	oglxColor4fv
  DESCRIPTION:
    Function shall changes current color if it is different 
    from previous one.
  SCOPE: global
  PARAMETERS: 
    par_pf_color-> pointer to color
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglxColor4fv(const SGLfloat * par_pf_color)
{
    if (glob_pr_sglStatemachine->b_static_sequence_started) {
        SGLfloat *loc_pr_current_color = &glob_pr_sglStatemachine->r_current_color.tf_current_color[0];
        loc_pr_current_color[0] = par_pf_color[0];
        loc_pr_current_color[1] = par_pf_color[1];
        loc_pr_current_color[2] = par_pf_color[2];
        loc_pr_current_color[3] = par_pf_color[3];

#ifndef ES2_DEV_ENV
        glColor4f(par_pf_color[0], par_pf_color[1], par_pf_color[2], par_pf_color[3]);
#endif
    }
    else {
        sgl_flt_bin loc_prev;
        sgl_flt_bin loc_new;
        SGLfloat *loc_pr_current_color = &glob_pr_sglStatemachine->r_current_color.tf_current_color[0];
        SGLbool loc_set = SGL_TRUE;

        /* Compare red component */
        loc_new.f = par_pf_color[0];
        loc_prev.f = loc_pr_current_color[0];
        if (loc_prev.l == loc_new.l) {
            /* Compare green component */
            loc_new.f = par_pf_color[1];
            loc_prev.f = loc_pr_current_color[1];
            if (loc_prev.l == loc_new.l) {
                /* Compare blue component */
                loc_new.f = par_pf_color[2];
                loc_prev.f = loc_pr_current_color[2];
                if (loc_prev.l == loc_new.l) {
                    /* Compare alpha component */
                    loc_new.f = par_pf_color[3];
                    loc_prev.f = loc_pr_current_color[3];
                    if (loc_prev.l == loc_new.l) {
                        /* There is no need to compare the alpha component */
                        loc_set = SGL_FALSE;
                    }
                }
            }
        }

        if (loc_set) {
            loc_pr_current_color[0] = par_pf_color[0];
            loc_pr_current_color[1] = par_pf_color[1];
            loc_pr_current_color[2] = par_pf_color[2];
            loc_pr_current_color[3] = par_pf_color[3];
#ifndef ES2_DEV_ENV
            glColor4f(par_pf_color[0], par_pf_color[1], par_pf_color[2], par_pf_color[3]);
#endif
        }
    }
    return;
}

/* End of File ***************************************************************/
