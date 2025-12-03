/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_next_higher_power.c
 DESCRIPTION       : Function that returns the next higher power of 2
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

#define MTH_MAX_POW_2 1073741824

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_next_higher_power2
  DESCRIPTION:
    Function shall return the next higher power of 2 of 
    given long value par_l_val in the range [par_l_min_pow2,par_l_max_pow2]. 
    Otherwise: 
    {
    If par_l_val is less or equal to par_l_min_pow2, the function shall return par_l_min_pow2.
    If par_l_val is greater or equal to par_l_max_pow2, the function shall return par_l_max_pow2.
    If at least one of the parameters is strictly negative, the function shall return 0. 
    If at least one of the parameters is strictly greater than MTH_MAX_POW_2, the function shall return -1. 
    If par_l_min_pow2 is greater to par_l_max_pow2, the function shall return -1. 
	Normal range of par_l_val, par_l_min_pow2 and par_l_max_pow2 is [0,MTH_MAX_POW_2]. 
    MTH_MAX_POW_2 is 1073741824. 
    Algorithm: 
	If par_l_val is in range [par_l_min_pow2 + 1, par_l_max_pow2 - 1], 
	the function shall set all bits on the right hand of the most significant bit of par_l_val to 1 
    and then increment it by one to obtain the next power of two (all bits set to 0 except the most 
    significant one). 
    }
  SCOPE: global
  PARAMETERS:
    par_l_val -> A positive value
	par_l_min_pow2 -> Minimum expected value for power of two
	par_l_max_pow2 -> Maximum expected value for power of two
  RETURN:
    SGLlong -> Next higher power of 2  
---------------------------------------------------------------------+*/
SGLlong mth_next_higher_power2(SGLlong par_l_val, SGLlong par_l_min_pow2, SGLlong par_l_max_pow2)
{
    /* internal data */
    SGLlong loc_l_result;
    SGLulong loc_ul_k;

    if ((par_l_val < 0) || (par_l_min_pow2 < 0) || (par_l_max_pow2 < 0)
        || (par_l_val > MTH_MAX_POW_2)
        || (par_l_min_pow2 > MTH_MAX_POW_2) || (par_l_max_pow2 > MTH_MAX_POW_2) || (par_l_min_pow2 > par_l_max_pow2)) {
        loc_l_result = 0;
    }
    else {
        if (par_l_val <= par_l_min_pow2) {
            loc_ul_k = (SGLulong) par_l_min_pow2;
        }
        else if (par_l_val >= par_l_max_pow2) {
            loc_ul_k = (SGLulong) par_l_max_pow2;
        }
        else {
            loc_ul_k = (SGLulong) par_l_val - 1U;

            loc_ul_k = (loc_ul_k >> 1) | loc_ul_k;
            loc_ul_k = (loc_ul_k >> 2) | loc_ul_k;
            loc_ul_k = (loc_ul_k >> 4) | loc_ul_k;
            loc_ul_k = (loc_ul_k >> 8) | loc_ul_k;
            loc_ul_k = (loc_ul_k >> 16) | loc_ul_k;
            loc_ul_k += 1U;
        }
        loc_l_result = (SGLlong) loc_ul_k;
    }


    return loc_l_result;
}

/* End of File ***************************************************************/
