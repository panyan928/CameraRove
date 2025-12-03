/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth.h
 DESCRIPTION       : mathematical component constants and structures declaration
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- +*/

#ifndef _MTH_H
#define _MTH_H

#include "sgl_types.h"

/* Limited numerical value of MTH component */
#define MTH_FLOAT_MAX 3.402823466e+38F

#define MTH_ANGLE_MAX 99999.0F


/* Different threshold used to draw arcs */
#define MTH_RADIUS_TOO_SMALL_MAX	1.0F
#define MTH_RADIUS_VERY_SMALL_MAX	10.0F
#define MTH_RADIUS_MEDIUM_SMALL_MAX	15.0F
#define MTH_RADIUS_SMALL_MAX		20.0F
#define MTH_RADIUS_MEDIUM_MAX		60.0F
#define MTH_RADIUS_LARGE_MAX		260.0F

/*
** Delta angle (in degree) : use for ArcSectors calculations and Crowns (angle steps)
*/
#define MTH_DELTA_I_VERY_SMALL	28.0F
#define MTH_DELTA_I_SMALL		17.0F
#define MTH_DELTA_I_MEDIUM		9.0F
#define MTH_DELTA_I_LARGE		5.0F
#define MTH_DELTA_I_VERY_LARGE	3.4F


/* Minimum Float value */
#define MTH_MIN_F				0.001F


#define MTH_MIN_ANGLE		0.0F
#define MTH_MAX_ANGLE		360.0F

#define MTH_MIN_LONG_CONV (-99999L)
#define MTH_MAX_LONG_CONV 99999L

/*
** Functions prototypes
*/

#define mth_f_abs(x) (((SGLfloat)(x) < 0.0F) ? -((SGLfloat)(x)) : ((SGLfloat)(x)))

#define mth_get_table_index(par_l_index, par_l_size) ( ((SGLlong)(par_l_index) < 0L) || ((SGLlong)(par_l_size) < 0L) ?  (SGLlong) 0L : \
((SGLlong)(par_l_index) >= (SGLlong)(par_l_size)) ? (SGLlong) ((par_l_size) - 1L) : (SGLlong)(par_l_index))

#define mth_f_to_ul(x) (((x) >= (SGLfloat) 0.0F) ? \
            (SGLulong) ((x) + (SGLfloat) 0.5F) : (SGLulong) (-(x) + (SGLfloat) 0.5F))

#define mth_get_angle_from_radius(x) (((x) < MTH_RADIUS_SMALL_MAX) ? MTH_DELTA_I_SMALL : (((x) < MTH_RADIUS_MEDIUM_MAX) ? MTH_DELTA_I_MEDIUM : (((x) < MTH_RADIUS_LARGE_MAX) ? MTH_DELTA_I_LARGE : MTH_DELTA_I_VERY_LARGE)))

extern SGLlong mth_f_to_l(SGLfloat p_f_value);

extern SGLlong mth_f_to_l_ceil(SGLfloat par_f_value);

extern SGLfloat mth_move_in_good_quadrant(SGLfloat par_f_angle, SGLfloat par_f_deltax, SGLfloat par_f_deltay);

extern SGLfloat mth_asin_radian(SGLfloat par_f_x);

extern SGLfloat mth_atan_degree(SGLfloat par_f_x);

extern SGLfloat mth_cos_degree(SGLfloat par_f_x);

extern SGLfloat mth_sin_degree(SGLfloat par_f_x);

extern void mth_cos_sin_degree(SGLfloat par_f_x, SGLfloat * par_pf_cos, SGLfloat * par_pf_sin);

extern SGLfloat mth_sqrtf(SGLfloat par_f_val);

extern SGLlong mth_next_higher_power2(SGLlong par_l_val, SGLlong par_l_min_pow2, SGLlong par_l_max_pow2);

extern SGLfloat mth_acos_degree(SGLfloat par_f_deltax, SGLfloat par_f_radius);

extern void mth_integer_to_string(SGLlong par_l_value, SGLbyte * par_b_out_string, SGLlong * par_l_length);

extern SGLlong mth_exponent(SGLlong par_l_value, SGLlong par_l_exponent);

extern SGLulong mth_strlen(SGLushort * par_pus_string, SGLulong par_ul_max_string_length);

extern SGLfloat mth_get_min_value(SGLulong par_ul_nb_entries, SGLfloat * par_pf_values);

extern SGLfloat mth_get_max_value(SGLulong par_ul_nb_entries, SGLfloat * par_pf_values);

extern SGLfloat mth_norm(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2);

extern SGLbool mth_is_special_character(SGLbyte * par_b_string);

extern SGLbool mth_is_hexa_character(SGLbyte par_b_char);

extern SGLbool mth_point_in_triangle(sgl_point * par_s_pt, sgl_point * par_s_v1, sgl_point * par_s_v2, sgl_point * par_s_v3);

extern SGLfloat mth_f_round(SGLfloat par_f_value);

#endif                          /* _MTH_H */
