/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_utils.c
 DESCRIPTION       : Utilitary functions that may be used by SCADE Display KCG
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

#define MIN_INT (-2147483647L -1)
#define ONE_DIV_TWO_PI 0.0027777778F

#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M10.6, 2000, "Rule M10.6 is not applied here as a direct boolean check <<if (my_bool)>> is easier to read.")
#endif


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluFloatIsEqual
  DESCRIPTION:
    This function shall compare the two given float values and determine if they are equal or not. 
     If abs(par_f_float_1 - par_f_float_2) < SGL_MIN_F the function shall return 1. 
     If abs(par_f_float_1 - par_f_float_2) >= SGL_MIN_F the function shall return 0. 
     SGL_MIN_F is 0.000001. 
  SCOPE: global
  PARAMETERS:
    par_f_float_1 -> First float to compare
    par_f_float_2 -> Second float to compare
  RETURN:
    SGLbool -> 1 if the two floats are equal with SGL_MIN_F precision, 0 otherwise
---------------------------------------------------------------------+*/
SGLbool sgluFloatIsEqual(SGLfloat par_f_float_1, SGLfloat par_f_float_2)
{
    SGLfloat loc_f_diff = (par_f_float_1 - par_f_float_2);
    SGLbool loc_b_return = SGL_FALSE;

    if ((loc_f_diff < SGL_MIN_F) && (loc_f_diff > -SGL_MIN_F)) {
        loc_b_return = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sgluGetTableIndex
  DESCRIPTION:
    This function shall check the given index is in the range [0, par_l_size - 1]
  SCOPE: global
  PARAMETERS:
    par_l_index -> the index to check
    par_l_size -> upper limit of index value + 1
  RETURN:	 
    SGLlong -> the resulting index: 
	0 if par_l_index < 0 or if par_l_size <= 0, 
	par_l_index if par_l_index is in range [0, par_l_size - 1], 
	par_l_size - 1 if par_l_index >= par_l_size 
---------------------------------------------------------------------+*/
SGLlong sgluGetTableIndex(SGLlong par_l_index, SGLlong par_l_size)
{
    /* Raise an error if table index or table size is incorrect */
    if ((par_l_index < 0L) || (par_l_size <= 0L) || (par_l_index >= par_l_size)) {
        oglxSetError(SGL_ERROR_SGLU_GETTABLEINDEX, (SGLulong) par_l_index);
    }
    else {
        /* Nothing to do */
    }
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.1, 4, "Rule M12.6.1 is not applied here as a macro is used")
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    return mth_get_table_index(par_l_index, par_l_size);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sgluLongDiv
  DESCRIPTION:
    This function shall perform the division between the two given values
  SCOPE: global
  PARAMETERS:
    par_l_long_1 -> divided value
    par_l_long_2 -> divisor value
  RETURN:	 
    SGLlong -> the resulting division: 
	par_l_long_2 if par_l_long_2 is 0
---------------------------------------------------------------------+*/
SGLlong sgluLongDiv(SGLlong par_l_long_1, SGLlong par_l_long_2)
{
    SGLlong loc_l_return = 0L;

    /* Raise an error if divisor is null */
    if ((par_l_long_2 == 0L) || ((par_l_long_1 == MIN_INT) && (par_l_long_2 == -1L))) {
        oglxSetError(SGL_ERROR_SGLU_LONGDIV, (SGLulong) par_l_long_2);
    }
    else {
        loc_l_return = par_l_long_1 / par_l_long_2;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sgluFloatDiv
  DESCRIPTION:
    This function shall perform the division between the two given values
  SCOPE: global
  PARAMETERS:
    par_f_float_1 -> divided value
    par_f_float_2 -> divisor value
  RETURN:	 
    SGLlong -> the resulting division: 
	par_f_long_2 if par_f_long_2 is 0
---------------------------------------------------------------------+*/
SGLfloat sgluFloatDiv(SGLfloat par_f_float_1, SGLfloat par_f_float_2)
{
    SGLfloat loc_f_return = 0.0F;
    SGLbool loc_b_equal_0 = sgluFloatIsEqual(par_f_float_2, 0.0F);

    /* Raise an error if divisor is null */
    if (loc_b_equal_0) {
        oglxSetError(SGL_ERROR_SGLU_FLOATDIV, 0U);
    }
    else {
        loc_f_return = par_f_float_1 / par_f_float_2;
    }

    return loc_f_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sgluModuloAngle
  DESCRIPTION:
    This function shall set the given angle between 0.0 and 360.0
  SCOPE: global
  PARAMETERS:
    par_f_angle -> the angle parameter
  RETURN:	 
    SGLfloat -> the resulted angle in range [0.0,360.0]
---------------------------------------------------------------------+*/
SGLfloat sgluModuloAngle(SGLfloat par_f_angle)
{
    SGLfloat loc_f_result = 0.0F;
#ifdef ATTOL_CRC_PRAGMA
#pragma attol crc_justify_all (Rule M12.6.2, 2, "Rule M12.6.2 is not applied here as a macro is used")
#endif
    SGLfloat loc_f_angle = mth_f_abs(par_f_angle);

    if (loc_f_angle > SGL_VALUE_MAX) {
        oglxSetError(SGL_ERROR_SGLU_MODULOANGLE, 0U);
    }
    else {
        SGLfloat loc_f_value = par_f_angle * ONE_DIV_TWO_PI;
        SGLlong loc_l_value = (SGLlong) loc_f_value;
        SGLbool loc_b_min_angle = sgluFloatIsEqual(par_f_angle, MTH_MIN_ANGLE);
        if (loc_b_min_angle) {
            loc_f_result = MTH_MIN_ANGLE;
        }
        else {
            loc_f_result = par_f_angle - (((SGLfloat) loc_l_value) * 360.0F);

            if (loc_f_result < 0.0F) {
                loc_f_result = loc_f_result + 360.0F;
            }
            else {
                /* Nothing to do */
            }

            loc_b_min_angle = sgluFloatIsEqual(loc_f_result, MTH_MIN_ANGLE);

            if (loc_b_min_angle) {
                loc_f_result = MTH_MAX_ANGLE;
            }
            else {
                /* Nothing to do */
            }
        }
    }
    return loc_f_result;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToInt32
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLint32
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLint32 -> the output SGLint32
---------------------------------------------------------------------+*/
SGLint32 sglFloatToInt32(SGLfloat par_f_value)
{
    SGLint32 loc_l_return = 0L;

    if ((par_f_value > SGL_MAX_INT32_AS_FLOAT) || (par_f_value < -SGL_MAX_INT32_AS_FLOAT)) {
        loc_l_return = par_f_value > 0.0F ? SGL_MAX_INT32 : SGL_MIN_INT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT32, 0UL);
    }
    else {

        loc_l_return = (SGLint32) par_f_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToInt8
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglFloatToInt8(SGLfloat par_f_value)
{
    SGLint8 loc_l_return = 0L;

    if ((par_f_value > SGL_MAX_INT8_AS_FLOAT) || (par_f_value < SGL_MIN_INT8_AS_FLOAT)) {
        loc_l_return = par_f_value > 0.0F ? SGL_MAX_INT8 : SGL_MIN_INT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_l_return = (SGLint8) par_f_value;
    }

    return loc_l_return;

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToInt16
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLint16
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglFloatToInt16(SGLfloat par_f_value)
{
    SGLint16 loc_l_return = 0L;

    if ((par_f_value > SGL_MAX_INT16_AS_FLOAT) || (par_f_value < SGL_MIN_INT16_AS_FLOAT)) {
        loc_l_return = par_f_value > 0.0F ? SGL_MAX_INT16 : SGL_MIN_INT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_l_return = (SGLint16) par_f_value;
    }

    return loc_l_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToInt8
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLdouble
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglDoubleToInt8(SGLdouble par_d_value)
{
    SGLint8 loc_l_return = 0L;

    if ((par_d_value > SGL_MAX_INT8_AS_DOUBLE) || (par_d_value < SGL_MIN_INT8_AS_DOUBLE)) {
        loc_l_return = par_d_value > 0.0 ? SGL_MAX_INT8 : SGL_MIN_INT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_l_return = (SGLint8) par_d_value;
    }

    return loc_l_return;

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToInt16
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLint16
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLdouble
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglDoubleToInt16(SGLdouble par_d_value)
{
    SGLint16 loc_l_return = 0L;

    if ((par_d_value > SGL_MAX_INT16_AS_DOUBLE) || (par_d_value < SGL_MIN_INT16_AS_DOUBLE)) {
        loc_l_return = par_d_value > 0.0 ? SGL_MAX_INT16 : SGL_MIN_INT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_l_return = (SGLint16) par_d_value;
    }

    return loc_l_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToUInt32
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLuint32
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLuint32 -> the output SGLuint32
---------------------------------------------------------------------+*/
SGLuint32 sglFloatToUInt32(SGLfloat par_f_value)
{
    SGLuint32 loc_ui_return = 0UL;

    if (par_f_value > SGL_MAX_UINT32_AS_FLOAT) {
        loc_ui_return = SGL_MAX_UINT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        if (par_f_value < 0.0F) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
        }
        else {
            loc_ui_return = (SGLuint32) par_f_value;
        }
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToUInt16
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglFloatToUInt16(SGLfloat par_f_value)
{
    SGLuint16 loc_ui_return = 0UL;

    if (par_f_value > SGL_MAX_UINT16_AS_FLOAT) {
        loc_ui_return = SGL_MAX_UINT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        if (par_f_value < 0.0F) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
        }
        else {
            loc_ui_return = (SGLuint16) par_f_value;
        }
    }

    return loc_ui_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToUInt8
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglFloatToUInt8(SGLfloat par_f_value)
{
    SGLuint8 loc_ui_return = 0;

    if (par_f_value > SGL_MAX_UINT8_AS_FLOAT) {
        loc_ui_return = SGL_MAX_UINT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        if (par_f_value < 0.0F) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
        }
        else {
            loc_ui_return = (SGLuint8) par_f_value;
        }
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToUInt32
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLuint32 by truncation
  SCOPE: global
  PARAMETERS:
    par_d_value -> the input SGLdouble
  RETURN:	 
    SGLuint32 -> the output SGLuint32
---------------------------------------------------------------------+*/
SGLuint32 sglDoubleToUInt32(SGLdouble par_d_value)
{
    SGLuint32 loc_ui_return = 0UL;

    if (par_d_value > SGL_MAX_UINT32_AS_DOUBLE) {
        loc_ui_return = SGL_MAX_UINT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        if (par_d_value < 0.0) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
        }
        else {
            loc_ui_return = (SGLuint32) par_d_value;
        }
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToUInt16
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLuint16 by truncation
  SCOPE: global
  PARAMETERS:
    par_d_value -> the input SGLdouble
  RETURN:	 
    SGLuint32 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglDoubleToUInt16(SGLdouble par_d_value)
{
    SGLuint16 loc_ui_return = 0UL;

    if (par_d_value > SGL_MAX_UINT16_AS_DOUBLE) {
        loc_ui_return = SGL_MAX_UINT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        if (par_d_value < 0.0) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
        }
        else {
            loc_ui_return = (SGLuint16) par_d_value;
        }
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToUInt8
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLuint8 by truncation
  SCOPE: global
  PARAMETERS:
    par_d_value -> the input SGLdouble
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglDoubleToUInt8(SGLdouble par_d_value)
{
    SGLuint8 loc_ui_return = 0UL;

    if (par_d_value > SGL_MAX_UINT8_AS_DOUBLE) {
        loc_ui_return = SGL_MAX_UINT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        if (par_d_value < 0.0) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
        }
        else {
            loc_ui_return = (SGLuint8) par_d_value;
        }
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToInt32
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLint32 by truncation
  SCOPE: global
  PARAMETERS:
    par_d_value -> the input SGLdouble
  RETURN:	 
    SGLint32 -> the output SGLint32
---------------------------------------------------------------------+*/
SGLint32 sglDoubleToInt32(SGLdouble par_d_value)
{
    SGLint32 loc_l_return = 0L;

    if ((par_d_value > SGL_MAX_INT32_AS_DOUBLE) || (par_d_value < SGL_MIN_INT32_AS_DOUBLE)) {
        loc_l_return = (par_d_value > 0.0 ? SGL_MAX_INT32 : SGL_MIN_INT32);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT32, 0UL);
    }
    else {
        loc_l_return = (SGLint32) par_d_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToFloat
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLfloat
  SCOPE: global
  PARAMETERS:
    par_d_value -> the input SGLdouble
  RETURN:	 
    SGLfloat -> the output SGLfloat
---------------------------------------------------------------------+*/
SGLfloat sglDoubleToFloat(SGLdouble par_d_value)
{
    SGLfloat loc_f_return = 0.0F;

    if ((par_d_value > SGL_MAX_FLOAT_AS_DOUBLE) || (par_d_value < -SGL_MAX_FLOAT_AS_DOUBLE)) {
        if (par_d_value > 0.0) {
            loc_f_return = SGL_MAX_FLOAT;
        }
        else {
            loc_f_return = -SGL_MAX_FLOAT;
        }
        oglxSetError(SGL_ERROR_SGL_CONVERTTOFLOAT, 0UL);
    }
    else {
        loc_f_return = (SGLfloat) par_d_value;
    }

    return loc_f_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt32ToInt32
  DESCRIPTION:
    This function shall convert a SGLuint32 to a SGLint32
  SCOPE: global
  PARAMETERS:
    par_ul_value -> the input SGLuint32
  RETURN:	 
    SGLint32 -> the output SGLint32
---------------------------------------------------------------------+*/
SGLint32 sglUInt32ToInt32(SGLuint32 par_ul_value)
{
    SGLint32 loc_l_return = 0L;

    if (par_ul_value > SGL_MAX_INT32_AS_UINT32) {
        loc_l_return = SGL_MAX_INT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT32, 0UL);
    }
    else {
        loc_l_return = (SGLint32) par_ul_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt16ToInt16
  DESCRIPTION:
    This function shall convert a SGLuint16 to a SGLint16
  SCOPE: global
  PARAMETERS:
    par_ul_value -> the input SGLuint16
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglUInt16ToInt16(SGLuint16 par_ui_value)
{
    SGLint16 loc_i_return = 0L;

    if (par_ui_value > SGL_MAX_INT16_AS_UINT16) {
        loc_i_return = SGL_MAX_INT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_i_return = (SGLint16) par_ui_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt8ToUInt32
  DESCRIPTION:
    This function shall convert a SGLint8 to a SGLuint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint8
  RETURN:	 
    SGLuint32 -> the output SGLuint32
---------------------------------------------------------------------+*/
SGLuint32 sglInt8ToUInt32(SGLint8 par_l_value)
{
    SGLuint32 loc_l_return = 0UL;

    if (par_l_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        loc_l_return = (SGLuint32) par_l_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt8ToUInt16
  DESCRIPTION:
    This function shall convert a SGLint8 to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint8
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglInt8ToUInt16(SGLint8 par_i_value)
{
    SGLuint16 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        loc_ui_return = (SGLuint16) par_i_value;
    }

    return loc_ui_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt8ToInt8
  DESCRIPTION:
    This function shall convert a SGLuint8 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint8
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglUInt8ToInt8(SGLuint8 par_ui_value)
{
    SGLint8 loc_i_return = 0;

    if (par_ui_value > SGL_MAX_INT8) {
        loc_i_return = SGL_MAX_INT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_ui_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt8ToUInt8
  DESCRIPTION:
    This function shall convert a SGLint8 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint8
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglInt8ToUInt8(SGLint8 par_i_value)
{
    SGLuint8 loc_ui_return = 0;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt16ToInt8
  DESCRIPTION:
    This function shall convert a SGLint16 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint16
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglInt16ToInt8(SGLint16 par_i_value)
{
    SGLint8 loc_i_return = 0;

    if ((par_i_value > SGL_MAX_INT8) || (par_i_value < SGL_MIN_INT8)) {
        loc_i_return = (par_i_value > 0 ? SGL_MAX_INT8 : SGL_MIN_INT8);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_i_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt32ToInt16
  DESCRIPTION:
    This function shall convert a SGLint32 to a SGLint16
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint32
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglInt32ToInt16(SGLint32 par_i_value)
{
    SGLint16 loc_i_return = 0;

    if ((par_i_value > SGL_MAX_INT16) || (par_i_value < SGL_MIN_INT16)) {
        loc_i_return = (par_i_value > 0 ? SGL_MAX_INT16 : SGL_MIN_INT16);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_i_return = (SGLint16) par_i_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt16ToUInt8
  DESCRIPTION:
    This function shall convert a SGLint16 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint16
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglInt16ToUInt8(SGLint16 par_i_value)
{
    SGLuint8 loc_ui_return = 0;

    if ((par_i_value < 0) || (par_i_value > SGL_MAX_UINT8)) {
        loc_ui_return = (par_i_value < 0 ? 0 : SGL_MAX_UINT8);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt32ToUInt16
  DESCRIPTION:
    This function shall convert a SGLint32 to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint16
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglInt32ToUInt16(SGLint32 par_i_value)
{
    SGLuint16 loc_ui_return = 0;

    if ((par_i_value < 0) || (par_i_value > SGL_MAX_UINT16)) {
        loc_ui_return = (par_i_value < 0 ? 0 : SGL_MAX_UINT16);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        loc_ui_return = (SGLuint16) par_i_value;
    }

    return loc_ui_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt16ToInt8
  DESCRIPTION:
    This function shall convert a SGLuint16 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint16
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglUInt16ToInt8(SGLuint16 par_ui_value)
{
    SGLint8 loc_i_return = 0;

    if (par_ui_value > SGL_MAX_INT8) {
        loc_i_return = SGL_MAX_INT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_ui_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt32ToInt16
  DESCRIPTION:
    This function shall convert a SGLuint16 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint32
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglUInt32ToInt16(SGLuint32 par_ui_value)
{
    SGLint16 loc_i_return = 0;

    if (par_ui_value > SGL_MAX_INT16) {
        loc_i_return = SGL_MAX_INT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_i_return = (SGLint16) par_ui_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt16ToUInt8
  DESCRIPTION:
    This function shall convert a SGLuint16 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint16
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglUInt16ToUInt8(SGLuint16 par_ui_value)
{
    SGLuint8 loc_ui_return = 0;

    if (par_ui_value > SGL_MAX_UINT8) {
        loc_ui_return = SGL_MAX_UINT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_ui_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt32ToUInt16
  DESCRIPTION:
    This function shall convert a SGLuint32 to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint32
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglUInt32ToUInt16(SGLuint32 par_ui_value)
{
    SGLuint16 loc_ui_return = 0;

    if (par_ui_value > SGL_MAX_UINT16) {
        loc_ui_return = SGL_MAX_UINT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        loc_ui_return = (SGLuint16) par_ui_value;
    }

    return loc_ui_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt32ToInt8
  DESCRIPTION:
    This function shall convert a SGLint32 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint32
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglInt32ToInt8(SGLint32 par_i_value)
{
    SGLint8 loc_i_return = 0;

    if ((par_i_value > SGL_MAX_INT8) || (par_i_value < -SGL_MAX_INT8)) {
        loc_i_return = (par_i_value > 0 ? SGL_MAX_INT8 : -SGL_MAX_INT8);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_i_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt32ToUInt8
  DESCRIPTION:
    This function shall convert a SGLint32 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint32
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglInt32ToUInt8(SGLint32 par_i_value)
{
    SGLuint8 loc_ui_return = 0;

    if ((par_i_value < 0) || (par_i_value > SGL_MAX_UINT8)) {
        loc_ui_return = (par_i_value < 0 ? 0 : SGL_MAX_UINT8);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt32ToInt8
  DESCRIPTION:
    This function shall convert a SGLuint32 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint32
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglUInt32ToInt8(SGLuint32 par_ui_value)
{
    SGLint8 loc_i_return = 0;

    if (par_ui_value > SGL_MAX_INT8) {
        loc_i_return = SGL_MAX_INT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_ui_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt32ToUInt8
  DESCRIPTION:
    This function shall convert a SGLuint32 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint32
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglUInt32ToUInt8(SGLuint32 par_ui_value)
{
    SGLuint8 loc_ui_return = 0;

    if (par_ui_value > SGL_MAX_UINT8) {
        loc_ui_return = SGL_MAX_UINT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_ui_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt16ToUInt32
  DESCRIPTION:
    This function shall convert a SGLint16 to a SGLuint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint16
  RETURN:	 
    SGLuint32 -> the output SGLuint32
---------------------------------------------------------------------+*/
SGLuint32 sglInt16ToUInt32(SGLint16 par_l_value)
{
    SGLuint32 loc_l_return = 0UL;

    if (par_l_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        loc_l_return = (SGLuint32) par_l_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt32ToUInt32
  DESCRIPTION:
    This function shall convert a SGLint32 to a SGLuint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint32
  RETURN:	 
    SGLuint32 -> the output SGLuint32
---------------------------------------------------------------------+*/
SGLuint32 sglInt32ToUInt32(SGLint32 par_i_value)
{
    SGLuint32 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        loc_ui_return = (SGLuint32) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt16ToUInt16
  DESCRIPTION:
    This function shall convert a SGLint16 to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint16
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglInt16ToUInt16(SGLint16 par_i_value)
{
    SGLuint16 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        loc_ui_return = (SGLuint16) par_i_value;
    }

    return loc_ui_return;
}


#ifndef OGLX_NO_64_BITS

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToInt8
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint64
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglInt64ToInt8(SGLint64 par_i_value)
{
    SGLint8 loc_i_return = 0;

    if ((par_i_value > SGL_MAX_INT8) || (par_i_value < SGL_MIN_INT8)) {
        loc_i_return = (par_i_value > 0 ? SGL_MAX_INT8 : SGL_MIN_INT8);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_i_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToInt16
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLint16
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint64
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglInt64ToInt16(SGLint64 par_i_value)
{
    SGLint16 loc_i_return = 0;

    if ((par_i_value > SGL_MAX_INT16) || (par_i_value < SGL_MIN_INT16)) {
        loc_i_return = (par_i_value > 0 ? SGL_MAX_INT16 : SGL_MIN_INT16);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_i_return = (SGLint16) par_i_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToUInt8
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint64
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglInt64ToUInt8(SGLint64 par_i_value)
{
    SGLuint8 loc_ui_return = 0;

    if ((par_i_value < 0) || (par_i_value > SGL_MAX_UINT8)) {
        loc_ui_return = (par_i_value < 0 ? 0 : SGL_MAX_UINT8);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToInt8
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint64
  RETURN:	 
    SGLint8 -> the output SGLint8
---------------------------------------------------------------------+*/
SGLint8 sglUInt64ToInt8(SGLuint64 par_ui_value)
{
    SGLint8 loc_i_return = 0;

    if (par_ui_value > SGL_MAX_INT8) {
        loc_i_return = SGL_MAX_INT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT8, 0UL);
    }
    else {
        loc_i_return = (SGLint8) par_ui_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToInt16
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLint16
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint64
  RETURN:	 
    SGLint16 -> the output SGLint16
---------------------------------------------------------------------+*/
SGLint16 sglUInt64ToInt16(SGLuint64 par_ui_value)
{
    SGLint16 loc_i_return = 0;

    if (par_ui_value > SGL_MAX_INT16) {
        loc_i_return = SGL_MAX_INT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT16, 0UL);
    }
    else {
        loc_i_return = (SGLint16) par_ui_value;
    }

    return loc_i_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToUInt8
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLuint8
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint64
  RETURN:	 
    SGLuint8 -> the output SGLuint8
---------------------------------------------------------------------+*/
SGLuint8 sglUInt64ToUInt8(SGLuint64 par_ui_value)
{
    SGLuint8 loc_ui_return = 0;

    if (par_ui_value > SGL_MAX_UINT8) {
        loc_ui_return = SGL_MAX_UINT8;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT8, 0UL);
    }
    else {
        loc_ui_return = (SGLuint8) par_ui_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToInt32
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint64
  RETURN:	 
    SGLint32 -> the output SGLint32
---------------------------------------------------------------------+*/
SGLint32 sglInt64ToInt32(SGLint64 par_l_value)
{
    SGLint32 loc_l_return = 0L;

    if ((par_l_value > SGL_MAX_INT32) || (par_l_value < SGL_MIN_INT32)) {
        loc_l_return = par_l_value > 0 ? SGL_MAX_INT32 : SGL_MIN_INT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT32, 0UL);
    }
    else {
        loc_l_return = (SGLint32) par_l_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToInt32
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLuint64
  RETURN:	 
    SGLint32 -> the output SGLint32
---------------------------------------------------------------------+*/
SGLint32 sglUInt64ToInt32(SGLuint64 par_l_value)
{
    SGLint32 loc_l_return = 0L;

    if (par_l_value > SGL_MAX_INT32) {
        loc_l_return = SGL_MAX_INT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT32, 0UL);
    }
    else {
        loc_l_return = (SGLint32) par_l_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToUInt16
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_i_value -> the input SGLint64
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglInt64ToUInt16(SGLint64 par_i_value)
{
    SGLuint16 loc_ui_return = 0;

    if ((par_i_value < 0) || (par_i_value > SGL_MAX_UINT16)) {
        loc_ui_return = (par_i_value < 0 ? 0 : SGL_MAX_UINT16);
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        loc_ui_return = (SGLuint16) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToUInt16
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLuint16
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint64
  RETURN:	 
    SGLuint16 -> the output SGLuint16
---------------------------------------------------------------------+*/
SGLuint16 sglUInt64ToUInt16(SGLuint64 par_ui_value)
{
    SGLuint16 loc_ui_return = 0;

    if (par_ui_value > SGL_MAX_UINT16) {
        loc_ui_return = SGL_MAX_UINT16;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT16, 0UL);
    }
    else {
        loc_ui_return = (SGLuint16) par_ui_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToUInt32
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLuint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint64
  RETURN:	 
    SGLuint32 -> the output SGLuint32
---------------------------------------------------------------------+*/
SGLuint32 sglInt64ToUInt32(SGLint64 par_l_value)
{
    SGLuint32 loc_l_return = 0UL;

    if (par_l_value > SGL_MAX_UINT32) {
        loc_l_return = SGL_MAX_UINT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        if (par_l_value < 0) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
        }
        else {
            loc_l_return = (SGLuint32) par_l_value;
        }
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToUInt32
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLint32
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLuint64
  RETURN:	 
    SGLint32 -> the output SGLint32
---------------------------------------------------------------------+*/
SGLuint32 sglUInt64ToUInt32(SGLuint64 par_l_value)
{
    SGLuint32 loc_l_return = 0UL;

    if (par_l_value > SGL_MAX_UINT32) {
        loc_l_return = SGL_MAX_UINT32;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT32, 0UL);
    }
    else {
        loc_l_return = (SGLuint32) par_l_value;
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt8ToUInt64
  DESCRIPTION:
    This function shall convert a SGLint8 to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint8
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLuint64 sglInt8ToUInt64(SGLint8 par_i_value)
{
    SGLuint64 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
    }
    else {
        loc_ui_return = (SGLuint64) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt16ToUInt64
  DESCRIPTION:
    This function shall convert a SGLint16 to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint16
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLuint64 sglInt16ToUInt64(SGLint16 par_i_value)
{
    SGLuint64 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
    }
    else {
        loc_ui_return = (SGLuint64) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt32ToUInt64
  DESCRIPTION:
    This function shall convert a SGLint32 to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint32
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLuint64 sglInt32ToUInt64(SGLint32 par_i_value)
{
    SGLuint64 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
    }
    else {
        loc_ui_return = (SGLuint64) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglInt64ToUInt64
  DESCRIPTION:
    This function shall convert a SGLint64 to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_l_value -> the input SGLint64
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLuint64 sglInt64ToUInt64(SGLint64 par_i_value)
{
    SGLuint64 loc_ui_return = 0UL;

    if (par_i_value < 0) {
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
    }
    else {
        loc_ui_return = (SGLuint64) par_i_value;
    }

    return loc_ui_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglUInt64ToInt64
  DESCRIPTION:
    This function shall convert a SGLuint64 to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLuint64
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLint64 sglUInt64ToInt64(SGLuint64 par_ui_value)
{
    SGLint64 loc_i_return = 0L;

    if (par_ui_value > SGL_MAX_INT64_AS_UINT64) {
        loc_i_return = SGL_MAX_INT64;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT64, 0UL);
    }
    else {
        loc_i_return = (SGLint64) par_ui_value;
    }

    return loc_i_return;
}

#ifndef OGLX_NO_FTOL2
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToUInt64
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_f_value -> the input SGLfloat
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLuint64 sglFloatToUInt64(SGLfloat par_f_value)
{
    SGLuint64 loc_ui_return = 0UL;

    if (par_f_value > SGL_MAX_UINT64_AS_FLOAT) {
        loc_ui_return = SGL_MAX_UINT64;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
    }
    else {
        if (par_f_value < 0.0F) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
        }
        else {
            loc_ui_return = (SGLuint64) par_f_value;
        }
    }

    return loc_ui_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToUInt64
  DESCRIPTION:
    This function shall convert a SGLdouble to a SGLuint64
  SCOPE: global
  PARAMETERS:
    par_d_value -> the input SGLdouble
  RETURN:	 
    SGLuint64 -> the output SGLuint64
---------------------------------------------------------------------+*/
SGLuint64 sglDoubleToUInt64(SGLdouble par_d_value)
{
    SGLuint64 loc_ui_return = 0UL;

    if (par_d_value > SGL_MAX_UINT64_AS_DOUBLE) {
        loc_ui_return = SGL_MAX_UINT64;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
    }
    else {
        if (par_d_value < 0.0) {
            oglxSetError(SGL_ERROR_SGL_CONVERTTOUINT64, 0UL);
        }
        else {
            loc_ui_return = (SGLuint64) par_d_value;
        }
    }

    return loc_ui_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglFloatToInt64
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLint64
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLfloat
  RETURN:	 
    SGLint64 -> the output SGLint64
---------------------------------------------------------------------+*/
SGLint64 sglFloatToInt64(SGLfloat par_f_value)
{
    SGLint64 loc_i_return = 0L;

    if ((par_f_value > SGL_MAX_INT64_AS_FLOAT) || (par_f_value < -SGL_MAX_INT64_AS_FLOAT)) {
        loc_i_return = par_f_value > 0.0F ? SGL_MAX_INT64 : SGL_MIN_INT64;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT64, 0UL);
    }
    else {
        loc_i_return = (SGLint64) par_f_value;
    }

    return loc_i_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  sglDoubleToInt64
  DESCRIPTION:
    This function shall convert a SGLfloat to a SGLint64
  SCOPE: global
  PARAMETERS:
    par_ui_value -> the input SGLfloat
  RETURN:	 
    SGLint64 -> the output SGLint64
---------------------------------------------------------------------+*/
SGLint64 sglDoubleToInt64(SGLdouble par_d_value)
{
    SGLint64 loc_i_return = 0L;

    if ((par_d_value > SGL_MAX_INT64_AS_DOUBLE) || (par_d_value < -SGL_MAX_INT64_AS_DOUBLE)) {
        loc_i_return = par_d_value > 0.0F ? SGL_MAX_INT64 : SGL_MIN_INT64;
        oglxSetError(SGL_ERROR_SGL_CONVERTTOINT64, 0UL);
    }
    else {
        loc_i_return = (SGLint64) par_d_value;
    }

    return loc_i_return;
}
#endif

#endif

/* End of File ***************************************************************/
