/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_asin_radian.c
 DESCRIPTION       : This function computes the principal value of the arc sine
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

#define MTH_PI_DIV_2  1.5707963268F
#define MTH_PI_DIV_4  0.7853981634F
#define MTH_EPS       2.44140625E-4F    /* B ** (-t / 2) */

#define MTH_P1        0.933935835F
#define MTH_P2        (-0.504401557F)
#define MTH_Q0        5.60363004F
#define MTH_Q1        (-5.54846723F)

/****************************************************************************** 
 **                           Module Body 
 *****************************************************************************/



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_asin_radian
  DESCRIPTION:
    This function shall compute an arc sine in radians. 
    {
	The function result shall be equivalent to asin(par_f_x) with a precision of 1.0e-5 if par_f_x is in its normal range [-1.0,1.0]. 
	If par_f_x is strictly less than -1.0 then the returned value shall be -MTH_PI_DIV_2.
    If par_f_x is strictly greater than 1.0 then the returned value shall be MTH_PI_DIV_2. 
    MTH_PI_DIV_2 is 1.5707963268. 
    }
    Algorithm: 
    The algorithm comes from "Software Manual for the Elementary Functions", 
    by William J. Cody Jr. and William Waite; Prentice Hall, 1980 
    pages 174-176 and 181-184: 
    As described in flow chart page 176 the algorithm shall be: 
    {				
	  
      Y = abs(par_f_x);
      if (Y > 1/2) {
        g = (1-Y) / 2;
        Y = -2 * sqrtf(g);
        Result = Y+Y*R(g);
      } else {
        g = Y*Y;
        Result = Y+Y*R(g);
      }
	  if (par_f_x < 0.0) {
	    Result = -Result;
	  }
    }.			
    R(g) is a rational approximation as described in note 9 pages 178-179, 
    it shall be computed as: 
    {
      MTH_P1 = 0.933935835F;
      MTH_P2 = -0.504401557F;
      MTH_Q0 = 5.60363004F;
      MTH_Q1 = -5.54846723F;
      R(g) = ((MTH_P2 * g + MTH_P1) * g) / (((g + MTH_Q1) * g) + MTH_Q0);
    }. 
  SCOPE: global
  PARAMETERS:
    par_f_x -> Value of the sine.
  RETURN:
    SGLfloat -> value of the arcsine for the passed argument in the range [-MTH_PI_DIV_2, MTH_PI_DIV_2].
---------------------------------------------------------------------+*/
SGLfloat mth_asin_radian(SGLfloat par_f_x)
{
/* let B be the radix (2 for our purpose) of the floating-point number */
/* system and t the number of base-B digits in the significand of a    */
/* floating-point number (24 for our purpose).                         */

    /* internal data */
    SGLfloat loc_f_y;
    SGLfloat loc_f_g;
    SGLfloat loc_f_gpg;
    SGLfloat loc_f_qg;
    SGLfloat loc_f_a;
    SGLfloat loc_f_result;

    /* start of code */
    if (par_f_x < -1.0F) {
        loc_f_result = -MTH_PI_DIV_2;
    }
    else {
        if (par_f_x > 1.0F) {
            loc_f_result = MTH_PI_DIV_2;
        }
        else {
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
            loc_f_y = (SGLfloat) mth_f_abs(par_f_x);
            if (loc_f_y > 0.5F) {
                /* 
                 * * evaluate G, step 1 (page 182 note 8) 
                 */
                loc_f_g = 0.5F - loc_f_y;
                /* step 2 */
                loc_f_g = loc_f_g + 0.5F;

                loc_f_g = loc_f_g * 0.5F;

                loc_f_y = mth_sqrtf(loc_f_g);
                loc_f_y = -(loc_f_y * 2.0F);

                loc_f_gpg = ((MTH_P2 * loc_f_g) + MTH_P1) * loc_f_g;
                loc_f_qg = ((loc_f_g + MTH_Q1) * loc_f_g) + MTH_Q0;
                /* Here loc_f_qg is at least ((0.5*0.5) + MTH_Q1) * (0.5*0.5) + MTH_Q0 */
                /* It cannot be near zero */
                loc_f_result = loc_f_y + (loc_f_y * (loc_f_gpg / loc_f_qg));
                loc_f_a = MTH_PI_DIV_4;
            }
            else {
                if (loc_f_y < MTH_EPS) {
                    loc_f_result = loc_f_y;
                }
                else {
                    loc_f_g = loc_f_y * loc_f_y;
                    loc_f_gpg = ((MTH_P2 * loc_f_g) + MTH_P1) * loc_f_g;
                    loc_f_qg = ((loc_f_g + MTH_Q1) * loc_f_g) + MTH_Q0;
                    /* Here loc_f_qg is at least MTH_Q0 if loc_f_y is near zero */
                    /* It cannot be near zero */
                    loc_f_result = loc_f_y + (loc_f_y * (loc_f_gpg / loc_f_qg));
                }
                loc_f_a = 0.0F;
            }
            /* 2 operations for accuracy (page 184 note 11) */
            loc_f_result = (loc_f_a + loc_f_result) + loc_f_a;
            if ((SGLfloat) par_f_x < 0.0F) {
                loc_f_result = -loc_f_result;
            }
            else {              /* Nothing to do */
            }
        }
    }
    return loc_f_result;
}

/* End of File ***************************************************************/
