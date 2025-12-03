/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglClipPlane.c
 DESCRIPTION       : sglClipPlane command shall specify a plane for clipping
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
#include "mth.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

#ifndef OGLX_NO_GLCLIPPLANE

/* Clip planes global structure */
SGLulong glob_ul_gl_clipplane_code[6];

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_clip_planes
  DESCRIPTION:
    Function shall disable all clip planes.
  SCOPE: local
  PARAMETERS:
    void
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_clip_planes(void)
{
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glDisable(GL_CLIP_PLANE5);

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_enable_clip_plane
  DESCRIPTION:
    Function shall enable given clip plane.
  SCOPE: local
  PARAMETERS:
    par_ul_cap -> clip plane id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_enable_clip_plane(SGLulong par_ul_cap)
{
    SGLbyte loc_b_number;
    loc_b_number = sgl_get_clip_plane_number((SGLlong) par_ul_cap);

    if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] != SGL_CP_OFF) {
        if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] == SGL_CP_AVAILABLE) {
            glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] = SGL_CP_ENABLE;
            glEnable(glob_ul_gl_clipplane_code[loc_b_number]);
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) par_ul_cap);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_clip_plane
  DESCRIPTION:
    Function shall disable given clip plane.
  SCOPE: local
  PARAMETERS:
    par_ul_cap -> clip plane id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_clip_plane(SGLulong par_ul_cap)
{
    /* Set clip plane status to disabled in OGLX context */
    SGLbyte loc_b_number;
    loc_b_number = sgl_get_clip_plane_number((SGLlong) par_ul_cap);
    if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] != SGL_CP_OFF) {
        if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] == SGL_CP_ENABLE) {
            glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] = SGL_CP_AVAILABLE;
            glDisable(glob_ul_gl_clipplane_code[loc_b_number]);
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_clip_plane_structure
  DESCRIPTION:
    Function shall initialize clip planes structure.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    void 
---------------------------------------------------------------------+*/
void sgl_initialize_clip_plane_structure(void)
{
    /* Initialize Clip plane table */
    glob_ul_gl_clipplane_code[0] = (SGLulong) GL_CLIP_PLANE0;
    glob_ul_gl_clipplane_code[1] = (SGLulong) GL_CLIP_PLANE1;
    glob_ul_gl_clipplane_code[2] = (SGLulong) GL_CLIP_PLANE2;
    glob_ul_gl_clipplane_code[3] = (SGLulong) GL_CLIP_PLANE3;
    glob_ul_gl_clipplane_code[4] = (SGLulong) GL_CLIP_PLANE4;
    glob_ul_gl_clipplane_code[5] = (SGLulong) GL_CLIP_PLANE5;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_get_clip_plane_number
  DESCRIPTION:
    Function shall specify a plane for clipping.
  SCOPE: global
  PARAMETERS:
    par_l_number -> Clipplane to define
  RETURN:
    SGLbyte -> the index of clip plane in global table  
---------------------------------------------------------------------+*/
SGLbyte sgl_get_clip_plane_number(SGLlong par_l_number)
{
    SGLbyte loc_b_result;

    switch (par_l_number) {
    case SGL_CLIP_PLANE0:
        loc_b_result = 0;
        break;
    case SGL_CLIP_PLANE1:
        loc_b_result = 1;
        break;
    case SGL_CLIP_PLANE2:
        loc_b_result = 2;
        break;
    case SGL_CLIP_PLANE3:
        loc_b_result = 3;
        break;
    case SGL_CLIP_PLANE4:
        loc_b_result = 4;
        break;
    default:
        /*case SGL_CLIP_PLANE5: */
        loc_b_result = 5;
        break;
    }

    return loc_b_result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_clip_plane
  DESCRIPTION:
    Function shall specify a plane for clipping.
  SCOPE: global
  PARAMETERS:
    par_l_number -> Clipplane to define, range: [0,5]
    par_pf_data -> Address of the array saving the clipplane equation
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sgl_clip_plane(SGLlong par_l_number, const SGLfloat * par_pf_data)
{
    SGLbyte loc_b_error_detected = 0U;

    /* Check if function is called between sglBegin/sglEnd */
    if (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if ((par_l_number != (SGLlong) SGL_CLIP_PLANE0) && (par_l_number != (SGLlong) SGL_CLIP_PLANE1)
        && (par_l_number != (SGLlong) SGL_CLIP_PLANE2) && (par_l_number != (SGLlong) SGL_CLIP_PLANE3)
        && (par_l_number != (SGLlong) SGL_CLIP_PLANE4) && (par_l_number != (SGLlong) SGL_CLIP_PLANE5)) {
        loc_b_error_detected = 1U;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLbyte loc_b_number;

        loc_b_number = sgl_get_clip_plane_number(par_l_number);

        /* There are SGL_MAX_CLIP_PLANES maximum */
        if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] != SGL_CP_ENABLE) {
            /* Store the plane equation */
            GLdouble loc_td_equation[4];
            loc_td_equation[0] = (GLdouble) par_pf_data[0];
            loc_td_equation[1] = (GLdouble) par_pf_data[1];
            loc_td_equation[2] = (GLdouble) 0.0;
            loc_td_equation[3] = (GLdouble) par_pf_data[3];

#ifndef ES11_DEV_ENV
            glClipPlane(glob_ul_gl_clipplane_code[loc_b_number], loc_td_equation);
#else
            glClipPlanef(glob_ul_gl_clipplane_code[loc_b_number], loc_td_equation);
#endif

            glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] = SGL_CP_AVAILABLE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CLIPPLANE, (SGLulong) par_l_number);
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglClipPlane
  DESCRIPTION:
    Function shall specify a plane for clipping.
  SCOPE: local
  PARAMETERS:
    par_l_number -> Clipplane to define, range: [0,5]
    par_f_start_point_x -> X coordinate of start point
    par_f_start_point_y -> Y coordinate of start point
    par_f_angle -> Angle of the clip plane
    par_b_clockwise -> Orientation of the angle (counter clocwise if SGL_FALSE, clockwise otherwise)
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglClipPlane(SGLlong par_l_number, SGLfloat par_f_start_point_x, SGLfloat par_f_start_point_y, SGLfloat par_f_angle, SGLbool par_b_clockwise)
{
    SGLfloat loc_pf_equation[4];
    SGLfloat loc_f_side;

    if (par_b_clockwise != SGL_TRUE) {
        loc_f_side = 1.0F;
    }
    else {
        loc_f_side = -1.0F;
    }

    if (sgluFloatIsEqual(par_f_angle, 0.0F)) {
        loc_pf_equation[0] = 0.0F;
        loc_pf_equation[1] = -loc_f_side;
        loc_pf_equation[2] = 0.0F;
        loc_pf_equation[3] = par_f_start_point_y * loc_f_side;
    }
    else {
        if (sgluFloatIsEqual(par_f_angle, 180.0F)) {
            loc_pf_equation[0] = 0.0F;
            loc_pf_equation[1] = loc_f_side;
            loc_pf_equation[2] = 0.0F;
            loc_pf_equation[3] = -par_f_start_point_y * loc_f_side;
        }
        else {
            if (sgluFloatIsEqual(par_f_angle, 90.0F)) {
                loc_pf_equation[0] = loc_f_side;
                loc_pf_equation[1] = 0.0F;
                loc_pf_equation[2] = 0.0F;
                loc_pf_equation[3] = -par_f_start_point_x * loc_f_side;
            }
            else {
                if (sgluFloatIsEqual(par_f_angle, 270.0F)) {
                    loc_pf_equation[0] = -loc_f_side;
                    loc_pf_equation[1] = 0.0F;
                    loc_pf_equation[2] = 0.0F;
                    loc_pf_equation[3] = par_f_start_point_x * loc_f_side;
                }
                else {
                    SGLfloat loc_f_sin = 0.0F;
                    SGLfloat loc_f_cos = 0.0F;
                    mth_cos_sin_degree(par_f_angle, &loc_f_cos, &loc_f_sin);

                    loc_pf_equation[0] = loc_f_sin * loc_f_side;
                    loc_pf_equation[1] = -loc_f_cos * loc_f_side;
                    loc_pf_equation[2] = 0.0F;
                    loc_pf_equation[3] = ((loc_f_cos * par_f_start_point_y) - (loc_f_sin * par_f_start_point_x)) * loc_f_side;
                }
            }
        }
    }

    sgl_clip_plane(par_l_number, loc_pf_equation);

    return;
}
#endif
/* End of File ***************************************************************/
