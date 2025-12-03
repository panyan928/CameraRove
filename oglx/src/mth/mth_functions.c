/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_functions.c
 DESCRIPTION       : Set of mathematical functions
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/*+ Public interfaces +*/
#include "sgl_types.h"
#include "sglConstants.h"
#include "sgl.h"

/*+ Protected interfaces +*/
#include "mth.h"

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M17.4, 2000, "Rule M17.4 is not applied here as we do not know by advance the dimensions of the fonts structure.")
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#pragma attol crc_justify_all (Rule M12.6.1, 2000, "Rule M12.6.1 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif

SGLfloat mth_f_round(SGLfloat par_f_value)
{
    SGLfloat loc_f_result = 0.0F;
    if (par_f_value > 0.0F) {
        loc_f_result = (SGLfloat) mth_f_to_l(par_f_value);
    }
    else {
        loc_f_result = -((SGLfloat) mth_f_to_l(par_f_value));
    }

    return loc_f_result;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_f_to_l
  DESCRIPTION:
    This function shall round given float to the closest long value and return the absolute value of it. 
    {
    If p_f_value is in range [0.0,MTH_MAX_LONG_CONV], function shall round p_f_value to the closest long value and return it. 
    If p_f_value is in range [MTH_MIN_LONG_CONV,0.0], function shall round p_f_value to the closest long value and return the absolute value of it. 
	If p_f_value is exactly between two integer values (decimal part is .5) the upper long value shall be returned.
    If p_f_value is strictly greater than MTH_MAX_LONG_CONV, function shall return MTH_MAX_LONG_CONV. 
    If p_f_value is strictly less than MTH_MIN_LONG_CONV, function shall return MTH_MIN_LONG_CONV. 
    MTH_MIN_LONG_CONV is -99999.
    MTH_MAX_LONG_CONV is 99999.
    }
  SCOPE: global
  PARAMETERS:
    p_f_value -> The value to convert.
  RETURN:
    SGLlong -> The result long value, range:[MTH_MIN_LONG_CONV,MTH_MAX_LONG_CONV].
---------------------------------------------------------------------+*/
SGLlong mth_f_to_l(SGLfloat p_f_value)
{
    SGLlong loc_l_return = 0;

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.4, 20, "Rule M10.4 is not applied here as algorithm needs the conversion is the goal of the function.")
#endif
    if (p_f_value < (SGLfloat) MTH_MIN_LONG_CONV) {
        loc_l_return = MTH_MIN_LONG_CONV;
    }
    else {
        if (p_f_value > (SGLfloat) MTH_MAX_LONG_CONV) {
            loc_l_return = MTH_MAX_LONG_CONV;
        }
        else {
            if (p_f_value >= 0.0F) {
                loc_l_return = (SGLlong) (p_f_value + 0.5F);
            }
            else {
                loc_l_return = (SGLlong) (-p_f_value + 0.5F);
            }
        }
    }
    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_f_to_l_ceil
  DESCRIPTION:
    This function shall compute the smallest long value that is still bigger than a given float (the ceil value). 
    If par_f_value is equal to an integer with a precision of SGL_MIN_F, this integer shall be returned.
    SGL_MIN_F is 0.000001
    The normal range of par_f_value is [MTH_MIN_LONG_CONV,MTH_MAX_LONG_CONV].
    {
	If par_f_value is less or equal than MTH_MIN_LONG_CONV the returned value shall be MTH_MIN_LONG_CONV.
	If par_f_value is greater or equal than MTH_MAX_LONG_CONV the returned value shall be MTH_MAX_LONG_CONV.
    MTH_MIN_LONG_CONV is -99999.
    MTH_MAX_LONG_CONV is 99999.
    }
  SCOPE: global
  PARAMETERS:
    par_f_value -> The input float.
  RETURN:     
    SGLlong -> The result ceil value, range: [MTH_MIN_LONG_CONV,MTH_MAX_LONG_CONV].
---------------------------------------------------------------------+*/
SGLlong mth_f_to_l_ceil(SGLfloat par_f_value)
{
    SGLlong loc_l_result = 0;

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.4, 25, "Rule M10.4 is not applied here as algorithm needs the conversion from float to long and is based on 32 bits format of these two types.")
#endif
    if (par_f_value <= (SGLfloat) MTH_MIN_LONG_CONV) {
        loc_l_result = MTH_MIN_LONG_CONV;
    }
    else {
        if (par_f_value >= (SGLfloat) MTH_MAX_LONG_CONV) {
            loc_l_result = MTH_MAX_LONG_CONV;
        }
        else {
            SGLlong loc_l_value = mth_f_to_l(par_f_value);
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 4, "Rule M12.6.2 is not applied here as a macro is used")
#endif
            SGLfloat loc_f_abs_value = mth_f_abs(par_f_value);
            if (mth_f_abs(loc_f_abs_value - ((SGLfloat) loc_l_value)) < SGL_MIN_F) {
                loc_l_result = (SGLlong) par_f_value;
            }
            else {
                if (par_f_value >= 0.0F) {
                    loc_l_result = (SGLlong) par_f_value + 1L;
                }
                else {
                    loc_l_result = (SGLlong) par_f_value;
                }
            }
        }
    }
    return loc_l_result;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_move_in_good_quadrant
  DESCRIPTION:
    This function shall modify given angle according to the 
    quadrant the starting point of an arc is positionned in. 
    The resulted angle shall be computed as follows: 
    {
    - If par_f_deltax is greater or equal to 0.0 and par_f_delta_y is strictly less than 0.0, the returned value shall be -par_f_angle;
    - If par_f_deltax is greater or equal to 0.0 and par_f_delta_y is greater or equal to 0.0, the returned value shall be par_f_angle;
    - If par_f_deltax is strictly less than 0.0 and par_f_delta_y is strictly less than 0.0, the returned value shall be par_f_angle - 180.0;
    - If par_f_deltax is strictly less than 0.0 and par_f_delta_y is greater or equal to 0.0, the returned value shall be 180.0 - par_f_angle;
    }
    {
    If we represent the inputs in four quadrants given by par_f_deltax and par_f_deltay (i.e. two vectors) the algorithm is equivalent to the 
    following scheme where the origin of the landmark 
    is the center of the arc. Each quadrant represents the possible positions 
    of starting point.

                            |											
    180.0 - par_f_angle		|		par_f_angle
    ________________________|__________________________
                            |                          
    par_f_angle - 180.0     |      -par_f_angle
                            |
	}
  SCOPE: global
  PARAMETERS:  
    par_f_angle -> original angle
    par_f_deltax -> difference between x coordinates of starting point and center of the arc
    par_f_deltay -> difference between y coordinates of starting point and center of the arc
  RETURN:
    SGLfloat -> the computed angle.  
---------------------------------------------------------------------+*/
SGLfloat mth_move_in_good_quadrant(SGLfloat par_f_angle, SGLfloat par_f_deltax, SGLfloat par_f_deltay)
{
    SGLfloat loc_f_angle = par_f_angle;

    if (par_f_deltax >= (SGLfloat) 0.0F) {
        if (par_f_deltay < (SGLfloat) 0.0F) {
            loc_f_angle = -loc_f_angle;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        loc_f_angle = (SGLfloat) 180.0F - loc_f_angle;
        if ((par_f_deltay) < (SGLfloat) 0.0F) {
            loc_f_angle = -loc_f_angle;
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_f_angle;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_integer_to_string
  DESCRIPTION:
    This function shall convert given integer to a string in given string. 
    It shall also fill the length of the string.
  SCOPE: global
  PARAMETERS:
    par_l_value -> the value to convert
    par_b_out_string -> the converted string, it is an output of the function.
    par_l_length -> the length of the output string, it is an output of the function.
  RETURN:	 
    None
---------------------------------------------------------------------+*/
void mth_integer_to_string(SGLlong par_l_value, SGLbyte * par_b_out_string, SGLlong * par_l_length)
{
    SGLlong loc_l_remainer;
    SGLlong loc_l_i;
    SGLlong loc_l_j;
    SGLbyte loc_b_str[256];
    SGLlong loc_l_length;
    SGLlong loc_l_digit;

    /* Conversion of the integer into a string */
    if (par_l_value == 0) {
        /* Default value for zero value */
        loc_l_length = 1;
        par_b_out_string[0] = (SGLbyte) 0x30;
    }
    else {
        /* Absolute value used in case of negativ value */
        if (par_l_value < 0) {
            par_l_value = -par_l_value;
        }
        else {
            /* Nothing to do */
        }

        loc_l_remainer = par_l_value;
        loc_l_length = 0;

        while (loc_l_remainer != 0) {
            loc_l_digit = loc_l_remainer % 10;
            /* Add current digit to the string, 0x30 is character '0' */
            loc_b_str[loc_l_length] = (SGLbyte) (loc_l_digit + 0x30);
            loc_l_length++;
            loc_l_remainer = loc_l_remainer / 10;
        }

        /* Revert the content of the string */
        loc_l_j = 0L;
        loc_l_i = loc_l_length;
        while (loc_l_i > 0) {
            loc_l_i--;
            par_b_out_string[loc_l_j] = loc_b_str[loc_l_i];
            loc_l_j++;

        }
    }

    /* Add a null character 0x0 */
    par_b_out_string[loc_l_length] = 0x0;

    /* Fill the length */
    *par_l_length = loc_l_length;

    return;

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_exponent
  DESCRIPTION:
    This function shall compute par_l_value^par_l_exponent.
  SCOPE: global
  PARAMETERS:
    par_l_value -> the value
    par_l_exponent -> the exponent
  RETURN:	 
    SGLlong -> par_l_value^par_l_exponent
---------------------------------------------------------------------+*/
SGLlong mth_exponent(SGLlong par_l_value, SGLlong par_l_exponent)
{
    SGLlong loc_l_return = 1L;
    SGLlong loc_l_i;

    if (par_l_value == 0L) {
        if (par_l_exponent == 0L) {
            /* loc_l_return = 1L; */
        }
        else {
            loc_l_return = 0L;
        }
    }
    else {
        if (par_l_exponent >= 0) {
            loc_l_i = par_l_exponent;
        }
        else {
            loc_l_i = -par_l_exponent;
        }

        while (loc_l_i > 0) {
            loc_l_i--;
            if (par_l_exponent >= 0) {
                loc_l_return = loc_l_return * par_l_value;
            }
            else {
                loc_l_return = loc_l_return / par_l_value;
            }
        }
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_is_hexa_character
  DESCRIPTION:
    This function shall check if given character is an hexadecimal one
  SCOPE: global
  PARAMETERS:
    par_b_char -> the input character
  RETURN:	 
    SGLbool -> SGL_TRUE if par_b_char is an hexadecimal character, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool mth_is_hexa_character(SGLbyte par_b_char)
{
    SGLbool loc_b_return = SGL_FALSE;

    if ((par_b_char >= 'A') && (par_b_char <= 'F')) {
        loc_b_return = SGL_TRUE;
    }
    else {
        if ((par_b_char >= 'a') && (par_b_char <= 'f')) {
            loc_b_return = SGL_TRUE;
        }
        else {
            if ((par_b_char >= '0') && (par_b_char <= '9')) {
                loc_b_return = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
        }
    }

    return loc_b_return;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 20, "Rule M16.7 is not applied here, the function ensures the content of the pointer is not modified.")
#endif
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_is_special_character
  DESCRIPTION:
    This function shall check if the input string starts with a special character
  SCOPE: global
  PARAMETERS:
    par_b_string -> the input string
  RETURN:	 
    SGLbool -> SGL_TRUE if par_b_string starts with "\uHHHH", H being 
    an hexadecimal character, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool mth_is_special_character(SGLbyte * par_b_string)
{
    SGLbool loc_l_return = SGL_FALSE;
    SGLbool loc_b_char2 = mth_is_hexa_character(par_b_string[2]);
    SGLbool loc_b_char3 = mth_is_hexa_character(par_b_string[3]);
    SGLbool loc_b_char4 = mth_is_hexa_character(par_b_string[4]);
    SGLbool loc_b_char5 = mth_is_hexa_character(par_b_string[5]);

    if ((par_b_string[0] == '\\')
        && (par_b_string[1] == 'u')
        && (loc_b_char2)
        && (loc_b_char3)
        && (loc_b_char4)
        && (loc_b_char5)) {
        loc_l_return = SGL_TRUE;
    }
    else {
        /* Nothing to to */
    }

    return loc_l_return;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 20, "Rule M16.7 is not applied here, the function ensures the content of the pointer is not modified.")
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_strlen
  DESCRIPTION:
    This function shall return the length of given string (the index of first chracter which value is 0).
    If there is no 0 character before par_ul_max_string_length, the function shall return par_ul_max_string_length.
  SCOPE: global
  PARAMETERS:
    par_pus_string -> the string
    par_ul_max_string_length -> the maximum length of the string
  RETURN:	 
    SGLulong -> the length of given string.
---------------------------------------------------------------------+*/
SGLulong mth_strlen(SGLushort * par_pus_string, SGLulong par_ul_max_string_length)
{
    SGLulong loc_ul_i = 0;

    while ((par_pus_string[loc_ul_i] != (SGLlong) 0x0) && (loc_ul_i < par_ul_max_string_length)) {
        loc_ul_i++;
    }

    return loc_ul_i;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 20, "Rule M16.7 is not applied here, the function ensures the content of the pointer is not modified.")
#endif
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_get_min_value
  DESCRIPTION:
    This function shall return the minimum value of given table
  SCOPE: global
  PARAMETERS:
    par_ul_nb_entries -> Number of entries in the table
	par_pf_values -> Table of values
  RETURN:	 
    SGLfloat -> the minimum value of the table
---------------------------------------------------------------------+*/
SGLfloat mth_get_min_value(SGLulong par_ul_nb_entries, SGLfloat * par_pf_values)
{
    SGLulong loc_ul_i;
    SGLfloat loc_f_result = par_pf_values[0];

    for (loc_ul_i = 1; loc_ul_i < par_ul_nb_entries; loc_ul_i++) {
        if (par_pf_values[loc_ul_i] < loc_f_result) {
            loc_f_result = par_pf_values[loc_ul_i];
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_f_result;
}

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M16.7, 20, "Rule M16.7 is not applied here, the function ensures the content of the pointer is not modified.")
#endif
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_get_max_value
  DESCRIPTION:
    This function shall return the maximum value of given table
  SCOPE: global
  PARAMETERS:
    par_ul_nb_entries -> Number of entries in the table
	par_pf_values -> Table of values
  RETURN:	 
    SGLfloat -> the maximum value of the table
---------------------------------------------------------------------+*/
SGLfloat mth_get_max_value(SGLulong par_ul_nb_entries, SGLfloat * par_pf_values)
{
    SGLulong loc_ul_i;
    SGLfloat loc_f_result = par_pf_values[0];

    for (loc_ul_i = 1; loc_ul_i < par_ul_nb_entries; loc_ul_i++) {
        if (par_pf_values[loc_ul_i] > loc_f_result) {
            loc_f_result = par_pf_values[loc_ul_i];
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_f_result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_norm
  DESCRIPTION:
    This function shall return the distance between the two given points
  SCOPE: global
  PARAMETERS:
    par_f_x1 -> X coordinate of first point
    par_f_y1 -> Y coordinate of first point
    par_f_x2 -> X coordinate of second point
    par_f_y2 -> Y coordinate of second point
  RETURN:	 
    SGLfloat -> the distance between the two points---------------------------------------------------------------------+*/
SGLfloat mth_norm(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLfloat loc_f_return;
    SGLfloat loc_f_dx = (par_f_x2 - par_f_x1);
    SGLfloat loc_f_dy = (par_f_y2 - par_f_y1);
    loc_f_return = (loc_f_dx * loc_f_dx) + (loc_f_dy * loc_f_dy);
    loc_f_return = mth_sqrtf(loc_f_return);

    return loc_f_return;
}


/*
Use of barycentric technique to compute position of point 

float alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
float beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
       ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
float gamma = 1.0f - alpha - beta;

p is inside (p1,p2,p3) if alpha >= 0 and beta >= 0 and gamma >= 0 
*/

SGLbool mth_point_in_triangle(sgl_point * par_s_pt, sgl_point * par_s_v1, sgl_point * par_s_v2, sgl_point * par_s_v3)
{
    SGLfloat loc_f_alpha =
        SGLfloat_div(((par_s_v2->f_y - par_s_v3->f_y) * (par_s_pt->f_x - par_s_v3->f_x) + (par_s_v3->f_x - par_s_v2->f_x) * (par_s_pt->f_y - par_s_v3->f_y)),
                     ((par_s_v2->f_y - par_s_v3->f_y) * (par_s_v1->f_x - par_s_v3->f_x) + (par_s_v3->f_x - par_s_v2->f_x) * (par_s_v1->f_y - par_s_v3->f_y)));
    SGLfloat loc_f_beta =
        SGLfloat_div(((par_s_v3->f_y - par_s_v1->f_y) * (par_s_pt->f_x - par_s_v3->f_x) + (par_s_v1->f_x - par_s_v3->f_x) * (par_s_pt->f_y - par_s_v3->f_y)),
                     ((par_s_v2->f_y - par_s_v3->f_y) * (par_s_v1->f_x - par_s_v3->f_x) + (par_s_v3->f_x - par_s_v2->f_x) * (par_s_v1->f_y - par_s_v3->f_y)));

    SGLfloat loc_f_gamma = 1.0F - loc_f_alpha - loc_f_beta;

    return (loc_f_alpha >= 0.0F) && (loc_f_beta >= 0.0F) && (loc_f_gamma >= 0.0F);
}

/* End of File ***************************************************************/
