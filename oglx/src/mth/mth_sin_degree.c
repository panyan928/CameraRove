/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_sin_degree.c
 DESCRIPTION       : Function that computes the sine of an angle in degrees
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
 ******************************************************************************/

/* let B be the radix (2 for our purpose) of the floating-point number */
/* system and t the number of base-B digits in the significand of a    */
/* floating-point number (24 for our purpose).                         */

/*
** B * (-t / 2)
*/
#define MTH_EPS         2.44140625E-4F
/*
** 1 / PI
*/
#define MTH_OVER_PI     5.55555556E-3F
/*
** 201 / 64 exact
*/
#define MTH_C1          3.140625F
/*
** representation is crucial
*/
#define MTH_C2       9.676535898E-4F
/* for t <= 24 */
#define MTH_R1       (-0.1666665668F)
#define MTH_R2       0.8333025139E-2F
#define MTH_R3       (-0.1980741872E-3F)
#define MTH_R4       0.2601903036E-5F

#define MTH_TWO_PI_OVER_360     0.017453292F


/******************************************************************************
 **                           Module Body
 *****************************************************************************/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_sin_degree
  DESCRIPTION:
    This function shall compute the sine of a value in degrees. 
    {
    The result of the function shall be equivalent to sin(par_f_x) with a precision of 1.0e-4 if par_f_x is in range [-MTH_ANGLE_MAX,MTH_ANGLE_MAX]. 
    If par_f_x is less than -MTH_ANGLE_MAX the returned value shall be 2.0. 
    If par_f_x is greater than MTH_ANGLE_MAX the returned value shall be 2.0. 
    }
    MTH_ANGLE_MAX is 99999.0. 
    {
    Algorithm: 
    From "Software Manual for the Elementary Functions", 
    by William J. Cody Jr. and William Waite; Prentice Hall, 1980 
    pages 125-128 and 134-138. 
    As described in flow chart pages 127-128 the algorithm shall be: 
    {
      sign = 1.0;
      Y = abs(par_f_x);
      N = INTRND(Y / PI);
      if (N is odd) {
        sign = -sign;
      }
      XN = FLOAT(N);
      f = ((Y * 2*PI/360.0) - (XN * MTH_C1)) - (XN * MTH_C2);
      if (abs(f) < MTH_EPS) {
        Result = sign * f;
      }
      else {
        g = f*f;
        Result = sign * (f + f*R(g));
      }
    }
    MTH_EPS is 2.44140625E-4.
    MTH_C1 is 3.140625.
    MTH_C2 is 9.676535898E-4.
    INTRND shall round the given value to nearest integer. 
    FLOAT shall convert given integer to a float. 
    R(g) is a rational approximation as described in note 13 p 131-132, 
    it shall be computed as: 
    {
      MTH_R1 = -0.1666665668;
      MTH_R2 = 0.8333025139E-2;
      MTH_R3 = -0.1980741872E-3;
      MTH_R4 = 0.2601903036E-5;
      R(g) = (((MTH_R4 * g + MTH_R3) * g + MTH_R2) * g + MTH_R1) * g;
    }
	}
  SCOPE: global
  PARAMETERS:
    par_f_x -> An angle in degrees. 
  RETURN:
    SGLfloat -> Sine of the parameter, range: [-1.0,1.0]
---------------------------------------------------------------------+*/
SGLfloat mth_sin_degree(SGLfloat par_f_x)
{
    /*
     * * internal data declaration
     */
    SGLfloat loc_f_result;
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    SGLfloat loc_f_y = (SGLfloat) mth_f_abs(par_f_x);
    /* start of code */

    if (loc_f_y > MTH_ANGLE_MAX) {
        loc_f_result = 2.0F;
    }
    else {
        /*
         * * compute N = round (Y * MTH_OVER_PI);
         */
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.4, 2, "Rule M10.4 is not applied here as algorithm needs the conversion from float to long and is based on 32 bits format of these two types.")
#endif
        SGLulong loc_ul_n = (SGLulong) ((loc_f_y * MTH_OVER_PI) + 0.5F);
        SGLfloat loc_f_f = (SGLfloat) loc_ul_n;
        loc_f_f = ((loc_f_y * MTH_TWO_PI_OVER_360) - (loc_f_f * MTH_C1)) - (loc_f_f * MTH_C2);
        /*
         * * avoid calling ABS(F)
         */
        if ((loc_f_f < MTH_EPS) && (loc_f_f > -MTH_EPS)) {
            if (par_f_x >= 0.0F) {
                loc_f_result = loc_f_f;
            }
            else {
                loc_f_result = -loc_f_f;
            }
            /* If loc_ul_n is odd */
            if ((loc_ul_n & 1U) == 1U) {
                loc_f_result = -loc_f_result;
            }
        }
        else {
            loc_f_result = loc_f_f * loc_f_f;
            loc_f_result = ((((((MTH_R4 * loc_f_result) + MTH_R3)
                               * loc_f_result) + MTH_R2) * loc_f_result) + MTH_R1) * loc_f_result;
            loc_f_result = (loc_f_f + (loc_f_f * loc_f_result));
            if (par_f_x < 0.0F) {
                loc_f_result = -loc_f_result;
            }
            /* If loc_ul_n is odd */
            if ((loc_ul_n & 1U) == 1U) {
                loc_f_result = -loc_f_result;
            }
        }
    }
    return loc_f_result;
}

/* End of File ***************************************************************/
