/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxColor4f.c
 DESCRIPTION       : Call to glColor4f if color changes
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
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxUpdateColor
  DESCRIPTION:
    Function shall change current color with given color index.
  SCOPE: global
  PARAMETERS:	    
    par_l_index -> color index, range: [0,glob_pr_sglStatemachine->ul_nb_colors - 1]
  RETURN:			
    void
---------------------------------------------------------------------+*/
void oglxUpdateColor(SGLlong par_l_index)
{
    sgl_color loc_sgl_colormap;
    SGLfloat loc_f_alpha;

    /* Check the index for robustness */
    if ((par_l_index < 0L) || (par_l_index > (SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1UL))) {
        par_l_index = (SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1UL);
    }
    else {
        /* Nothing to do */
    }

    loc_sgl_colormap = (glob_pr_sglStatemachine->tr_color)[par_l_index];

    /* Update current color */
    /* loc_f_alpha = ((GLfloat) glob_pr_sglStatemachine->b_transparency) / ((GLfloat) (SGL_MAX_ALPHA_8)); */
    loc_f_alpha = ((GLfloat) glob_pr_sglStatemachine->b_transparency) * SGL_ONE_DIV_MAX_ALPHA_8;

    /* Manage Reverse Video */
    if (!(glob_pr_sglStatemachine->b_reverse_color)) {
        oglxColor4f((GLfloat) loc_sgl_colormap.f_red, (GLfloat) loc_sgl_colormap.f_green, (GLfloat) loc_sgl_colormap.f_blue,
                    (GLfloat) loc_sgl_colormap.f_alpha * loc_f_alpha);
    }
    else {
        oglxColor4f((GLfloat) (1.0F - loc_sgl_colormap.f_red), (GLfloat) (1.0F - loc_sgl_colormap.f_green), (GLfloat) (1.0F - loc_sgl_colormap.f_blue),
                    loc_f_alpha);
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxColor4f
  DESCRIPTION:
    Function shall change current color if it is different 
    from previous one.
  SCOPE: global
  PARAMETERS:	    
    par_f_red -> Red component of the color, range: [0.0,1.0]
    par_f_green -> Green component of the color, range: [0.0,1.0]
    par_f_blue -> Blue component of the color, range: [0.0,1.0]
    par_f_alpha -> Alpha component of the color, range: [0.0,1.0]
  RETURN:			
    void
---------------------------------------------------------------------+*/
void oglxColor4f(SGLfloat par_f_red, SGLfloat par_f_green, SGLfloat par_f_blue, SGLfloat par_f_alpha)
{
    if (glob_pr_sglStatemachine->b_static_sequence_started) {
        SGLfloat *loc_pr_current_color = &glob_pr_sglStatemachine->r_current_color.tf_current_color[0];
        loc_pr_current_color[0] = par_f_red;
        loc_pr_current_color[1] = par_f_green;
        loc_pr_current_color[2] = par_f_blue;
        loc_pr_current_color[3] = par_f_alpha;

#ifndef ES2_DEV_ENV
        glColor4f(par_f_red, par_f_green, par_f_blue, par_f_alpha);
#endif
    }
    else {
        sgl_flt_bin loc_prev;
        sgl_flt_bin loc_new;
        SGLfloat *loc_pr_current_color = &glob_pr_sglStatemachine->r_current_color.tf_current_color[0];
        SGLbool loc_set = SGL_TRUE;

        /* Compare red component */
        loc_new.f = par_f_red;
        loc_prev.f = loc_pr_current_color[0];
        if (loc_prev.l == loc_new.l) {
            /* Compare green component */
            loc_new.f = par_f_green;
            loc_prev.f = loc_pr_current_color[1];
            if (loc_prev.l == loc_new.l) {
                /* Compare blue component */
                loc_new.f = par_f_blue;
                loc_prev.f = loc_pr_current_color[2];
                if (loc_prev.l == loc_new.l) {
                    /* Compare alpha component */
                    loc_new.f = par_f_alpha;
                    loc_prev.f = loc_pr_current_color[3];
                    if (loc_prev.l == loc_new.l) {
                        loc_set = SGL_FALSE;
                    }
                }
            }
        }

        if (loc_set) {
            loc_pr_current_color[0] = par_f_red;
            loc_pr_current_color[1] = par_f_green;
            loc_pr_current_color[2] = par_f_blue;
            loc_pr_current_color[3] = par_f_alpha;
#ifndef ES2_DEV_ENV
            glColor4f(par_f_red, par_f_green, par_f_blue, par_f_alpha);
#endif
        }
    }
    return;
}

/* End of File ***************************************************************/
