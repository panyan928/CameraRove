/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_atan_degree.c
 DESCRIPTION       : This function computes the principal value of the arc tangent
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl_types.h"

/*+ Protected interfaces +*/
#include "mth.h"

/******************************************************************************
 **                           Local file declarations
 *****************************************************************************/

/* let B be the radipar_f_x (2 for our purpose) of the floating-point number */
/* system and t the number of base-B digits in the significand of a    */
/* floating-point number (24 for our purpose).                         */
/*
** 2 - SQRT (3)
*/
#define MTH_C1        0.2679491924F
#define MTH_SQRT_3    1.7320508076F
/*
** SQRT (3) - 1
*/
#define MTH_SQRT_3_M1 0.7320508076F
/*
** B ** (-t / 2)
*/
#define MTH_EPS       2.44140625E-4F
#define MTH_A0        0.0F
/*
** PI / 6
*/
#define MTH_A1        30.0F
/*
** PI / 2
*/
#define MTH_A2        90.0F
/*
** PI / 3
*/
#define MTH_A3        60.0F
/*
** for t <= 24
*/
#define MTH_P0        (-0.4708325141F)
#define MTH_P1        (-0.5090958253E-1F)
#define MTH_Q0        1.412501740F
/*
** Convert radian to degree macro
*/
#define MTH_FLT_RAD_TO_DEG(X)  ((X) * (SGLfloat) 57.2957795130824F)


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_atan_degree
  DESCRIPTION:
    This function shall compute an arc tangent in degrees. 
    {
    The returned value shall be equivalent to atan(par_f_x) with a precision of 1.0e-4. 
	The range of par_f_x shall be [-MTH_FLOAT_MAX,MTH_FLOAT_MAX]. 
    }
    Algorithm: 
    From "Software Manual for the Elementary Functions", 
    by William J. Cody Jr. and William Waite; Prentice Hall, 1980 
    pages 194-197 and 203-206. 
    As described in flow chart page 196 the algorithm shall be: 
    {	
      f = abs(par_f_x);
      if (par_f_x < 0) {
        sign = -1.0;
      }
	  else {
        sign = 1.0;
      }
      if (f > 1) {
        f = 1/f;
        N = 2;
      }
      else {
        N = 0;
      }
      if (f > (2 - sqrtf(3))) {
        f = (f * sqrt(3) -1) / (sqrtf(3) + f);
        N = N+1;
      }
      if (abs(f) < MTH_EPS) {
        Result = f;
      }
      else {
        g = f*f;
        Result = f + f*R(g);
      }
    }
    MTH_EPS is 2.44140625E-4.
    R(g) is a rational approximation as described in note 13 p 200, 
    it shall be computed as: 
    {
      MTH_P0 = -0.4708325141;
      MTH_P1 = -0.5090958253E-1;
      MTH_Q0 = 1.412501740;
      R(g) = (((MTH_P1 * g) + MTH_P0) * g) / (g + MTH_Q0);
    }
    To have the final result in degrees: 
    {
      Result = RAD_TO_DEG(Result);
      if (N = 0) {
        Result = Result * sign;
      }
      if (N = 1) {
        Result = (Result + 30.0) * sign;
      }
      if (N = 2) {
        Result = (-Result + 90.0) * sign;
      }
      if (N = 3) {
        Result = (-Result + 60.0) * sign;
      }
    }
  SCOPE: global
  PARAMETERS:
    par_f_x -> Tangent value.
  RETURN:
    SGLfloat -> Value of the arc tangent for the argument in the range  [-90.0, +90.0] degrees.
---------------------------------------------------------------------+*/
SGLfloat mth_atan_degree(SGLfloat par_f_x)
{
    /* internal data */
    SGLfloat loc_f_f = par_f_x;
    SGLfloat loc_f_signe = 1.0F;
    SGLfloat loc_f_tf;
    SGLfloat loc_f_g;
    SGLfloat loc_f_r;
    SGLfloat loc_f_result;
    SGLlong loc_l_n;

    /* start of code */
    if (loc_f_f < 0.0F) {
        loc_f_signe = -1.0F;
        loc_f_f = -loc_f_f;
    }
    else {
        /* Already set */
    }

    loc_l_n = 0L;
    if (loc_f_f > 1.0F) {
        loc_l_n = 2L;
        loc_f_f = 1.0F / loc_f_f;
    }
    else {
        /* do nothing */
    }

    if (loc_f_f > MTH_C1) {     /* if loc_f_f > 2 - sqrt (3) */
        loc_l_n = loc_l_n + 1L;
        loc_f_tf = (MTH_SQRT_3_M1 * loc_f_f) - 0.5F;
        loc_f_tf = loc_f_tf - 0.5F;
        loc_f_tf = loc_f_tf + loc_f_f;
        loc_f_f = loc_f_tf / (MTH_SQRT_3 + loc_f_f);
    }
    else {
        /* do nothing */
    }

    /* avoid calling FABSF (loc_f_f) */
    if ((loc_f_f < MTH_EPS) && (loc_f_f > -MTH_EPS)) {
        loc_f_result = MTH_FLT_RAD_TO_DEG(loc_f_f);
    }
    else {
        loc_f_g = loc_f_f * loc_f_f;
        loc_f_r = (((MTH_P1 * loc_f_g) + MTH_P0) * loc_f_g) / (loc_f_g + MTH_Q0);
        loc_f_result = MTH_FLT_RAD_TO_DEG(loc_f_f + (loc_f_f * loc_f_r));
    }

    switch (loc_l_n) {
    case 0L:
    {
        loc_f_result = (loc_f_result + MTH_A0) * loc_f_signe;
    }
        break;

    case 1L:
    {
        loc_f_result = (loc_f_result + MTH_A1) * loc_f_signe;
    }
        break;

    case 2L:
    {
        loc_f_result = (-loc_f_result + MTH_A2) * loc_f_signe;
    }
        break;

    default:
    {
        loc_f_result = (-loc_f_result + MTH_A3) * loc_f_signe;
    }
        break;
    }

    return loc_f_result;
}

/* End of File ***************************************************************/
