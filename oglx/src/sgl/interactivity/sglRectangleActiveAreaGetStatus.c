/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglRectangleActiveAreaGetStatus.c
 DESCRIPTION       : sglRectangleActiveAreaGetStatus command shall return the 
					status of the mouse position: SGL_TRUE is returned if the mouse 
					is inside the rectangle active area, SGL_FALSE is returned if 
					the mouse is outside the rectangle active area
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
  NAME: sglRectangleActiveAreaGetStatus
  DESCRIPTION:
    Function shall return the status of the mouse position: SGL_TRUE is 
    returned if the mouse is inside the rectangle active area, SGL_FALSE 
    is returned if the mouse is outside the rectangle active area.
  SCOPE: global
  PARAMETERS:
    par_f_mouse_x -> x-coordinate for mouse
    par_f_mouse_y -> y-coordinate for mouse
    par_f_x1 -> x-coordinate for first point
    par_f_y1 -> y-coordinate for first point
    par_f_x2 -> x-coordinate for second point
    par_f_y2 -> y-coordinate for second point            
  RETURN:
    SGLbyte -> SGL_TRUE: the mouse is in the rectangle
    SGL_FALSE: the mouse is out of the rectangle  
---------------------------------------------------------------------+*/
SGLbool sglRectangleActiveAreaGetStatus(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2,
                                        SGLfloat par_f_y2)
{
    SGLbyte loc_b_return = SGL_FALSE;

    SGLfloat loc_tf_points[4][2];

    if ((((par_f_x2 - par_f_x1) > -SGL_MIN_F) && ((par_f_x2 - par_f_x1) < SGL_MIN_F))
        || (((par_f_y2 - par_f_y1) > -SGL_MIN_F) && ((par_f_y2 - par_f_y1) < SGL_MIN_F))) {
        /* Nothing to do */
    }
    else {
        /* First point */
        loc_tf_points[0][0] = par_f_x1;
        loc_tf_points[0][1] = par_f_y1;

        /* Second point */
        loc_tf_points[1][0] = par_f_x1;
        loc_tf_points[1][1] = par_f_y2;

        /* Third point */
        loc_tf_points[2][0] = par_f_x2;
        loc_tf_points[2][1] = par_f_y2;

        /* Fourth point */
        loc_tf_points[3][0] = par_f_x2;
        loc_tf_points[3][1] = par_f_y1;

        /* The status is given by function sglShapeActiveAreaGetStatus */
        loc_b_return = sglShapeActiveAreaGetStatus(par_f_mouse_x, par_f_mouse_y, 4L, loc_tf_points);
    }

    return loc_b_return;
}

/* End of File ***************************************************************/
