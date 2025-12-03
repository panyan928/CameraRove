/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : animation.c
 DESCRIPTION       : Animation of SCADE Display plugs
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

/* Includes */

#include "animation.h"

/* Animation functions for plug PN_AIRSPEED */
float _STEP_PN_AIRSPEED;

void init_PN_AIRSPEED(float * p_data ){
    _STEP_PN_AIRSPEED = (float)0.5;
    *p_data = (float)1.0;
}

void animate_PN_AIRSPEED(float * p_data ){
    *p_data += _STEP_PN_AIRSPEED;
    if (*p_data >= 200.0) {
        *p_data = (float)200.0;
        _STEP_PN_AIRSPEED = -_STEP_PN_AIRSPEED;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_AIRSPEED = -_STEP_PN_AIRSPEED;
    }
}


///* Animation functions for plug PN_ROLL_ANGLE */

float _STEP_PN_ROLL_ANGLE;

void init_PN_ROLL_ANGLE(float *p_data)
{
    _STEP_PN_ROLL_ANGLE = (float) 0.1;
    *p_data = (float) 0.3;
}

void animate_PN_ROLL_ANGLE(float *p_data)
{
    *p_data += _STEP_PN_ROLL_ANGLE;
    if (*p_data >= 10.0) {
        *p_data = (float) 10.0;
        _STEP_PN_ROLL_ANGLE = -_STEP_PN_ROLL_ANGLE;
    }
    else if (*p_data <= -10.0) {
        *p_data = (float) -10.0;
        _STEP_PN_ROLL_ANGLE = -_STEP_PN_ROLL_ANGLE;
    }
}


/* Animation functions for plug PN_ALTI */

float _STEP_PN_ALTI;

void init_PN_ALTI(float *p_data)
{
    _STEP_PN_ALTI = (float) 0.5;
    *p_data = (float) 1.0;
}

void animate_PN_ALTI(float *p_data)
{
    *p_data += _STEP_PN_ALTI;
    if (*p_data >= 600.0) {
        *p_data = (float) 600.0;
        _STEP_PN_ALTI = -_STEP_PN_ALTI;
    }
    else if (*p_data <= 0.0) {
        *p_data = (float) 0.0;
        _STEP_PN_ALTI = -_STEP_PN_ALTI;
    }
}


/* Animation functions for plug PN_PITCH_ANGLE */

float _STEP_PN_PITCH_ANGLE;

void init_PN_PITCH_ANGLE(float *p_data)
{
    _STEP_PN_PITCH_ANGLE = (float) 0.1;
    *p_data = (float) 0.2;
}

void animate_PN_PITCH_ANGLE(float *p_data)
{
    *p_data += _STEP_PN_PITCH_ANGLE;
    if (*p_data >= 15.0) {
        *p_data = (float) 15.0;
        _STEP_PN_PITCH_ANGLE = -_STEP_PN_PITCH_ANGLE;
    }
    else if (*p_data <= -15.0) {
        *p_data = (float) -15.0;
        _STEP_PN_PITCH_ANGLE = -_STEP_PN_PITCH_ANGLE;
    }
}
/* Animation functions for plug TAS */

float _STEP_PN_TAS;

void init_PN_TAS(float * p_data ){
    _STEP_PN_TAS = (float)0.5;
    *p_data = (float)1.0;
}

void animate_PN_TAS(float * p_data ){
    *p_data += _STEP_PN_TAS;
    if (*p_data >= 500.0) {
        *p_data = (float)500.0;
        _STEP_PN_TAS = -_STEP_PN_TAS;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_TAS = -_STEP_PN_TAS;
    }
}


/* Animation functions for plug GS */

float _STEP_PN_GS;

void init_PN_GS(float * p_data ){
    _STEP_PN_GS = (float)0.5;
    *p_data = (float)1.0;
}

void animate_PN_GS(float * p_data ){
    *p_data += _STEP_PN_GS;
    if (*p_data >= 500.0) {
        *p_data = (float)500.0;
        _STEP_PN_GS = -_STEP_PN_GS;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_GS = -_STEP_PN_GS;
    }
}


/* Animation functions for plug F_ALT_VALUE */

float _STEP_PN_F_ALT_VALUE;

void init_PN_F_ALT_VALUE(float * p_data ){
    _STEP_PN_F_ALT_VALUE = (float)0.5;
    *p_data = (float)1.0;
}

void animate_PN_F_ALT_VALUE(float * p_data ){
    *p_data += _STEP_PN_F_ALT_VALUE;
    if (*p_data >= 500.0) {
        *p_data = (float)500.0;
        _STEP_PN_F_ALT_VALUE = -_STEP_PN_F_ALT_VALUE;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_F_ALT_VALUE = -_STEP_PN_F_ALT_VALUE;
    }
}

/* Animation functions for plug HDG */

float _STEP_PN_HDG;

void init_PN_HDG(float * p_data ){
    _STEP_PN_HDG = (float)0.25;
    *p_data = (float)1.0;
}

void animate_PN_HDG(float * p_data,float *ndthg ){
    *p_data += _STEP_PN_HDG;
    if (*p_data >= 360.0) {
        *p_data = (float)360.0;
        _STEP_PN_HDG = -_STEP_PN_HDG;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_HDG = -_STEP_PN_HDG;
    }
    *ndthg = *p_data;
}

/* Animation functions for plug NAV_VALUE */

float _STEP_PN_NAV_VALUE;

void init_PN_NAV_VALUE(float * p_data ){
    _STEP_PN_NAV_VALUE = (float)0.25;
    *p_data = (float)1.0;
}

void animate_PN_NAV_VALUE(float * p_data ){
    *p_data += _STEP_PN_NAV_VALUE;
    if (*p_data >= 360.0) {
        *p_data = (float)360.0;
        _STEP_PN_NAV_VALUE = -_STEP_PN_NAV_VALUE;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_NAV_VALUE = -_STEP_PN_NAV_VALUE;
    }
}

float _STEP_PN_F_NAVROSE_ANGLE;

void init_PN_F_NAVROSE_ANGLE(float * p_data ){
    _STEP_PN_F_NAVROSE_ANGLE = (float)0.25;
    *p_data = (float)1.0;
}

void animate_PN_F_NAVROSE_ANGLE(float * p_data ){
    *p_data += _STEP_PN_F_NAVROSE_ANGLE;
    if (*p_data >= 360.0) {
        *p_data = (float)360.0;
        _STEP_PN_F_NAVROSE_ANGLE = -_STEP_PN_F_NAVROSE_ANGLE;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_F_NAVROSE_ANGLE = -_STEP_PN_F_NAVROSE_ANGLE;
    }
}

float _STEP_PN_WIND;

void init_PN_WIND(float * p_data ){
    _STEP_PN_WIND = (float)1.0;
    *p_data = (float)0.0;
}

void animate_PN_WIND(float * p_data ){
    *p_data += _STEP_PN_WIND;
    if (*p_data >= 360.0) {
        *p_data = (float)360.0;
        _STEP_PN_WIND = -_STEP_PN_WIND;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_PN_WIND = -_STEP_PN_WIND;
    }
}



/* Animation functions for plug EPU_TRQ */
float _STEP_EPU_TRQ=0.1;
float epu_trq=0.0;

void animate_EPU_TRQ(float * p_data ){
    *p_data += _STEP_EPU_TRQ;
    if (*p_data >= 110.0) {
        *p_data = (float)110.0;
        _STEP_EPU_TRQ = -_STEP_EPU_TRQ;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_EPU_TRQ = -_STEP_EPU_TRQ;
    }
    epu_trq = *p_data;
}

void animate_EPU_TRQ_COLOR(int * p_data,int * p_fontcolor,int * p_bkgcolor ){
    if (epu_trq < 94.0) {
        *p_data = 11;
        *p_fontcolor = 11;
        *p_bkgcolor = 1;
    }
    else if (epu_trq < 100.0) {
        *p_data = 46;
        *p_fontcolor = 1;
        *p_bkgcolor = 46;
    }
    else if (epu_trq < 111.0) {
        *p_data = 36;
        *p_fontcolor = 60;
        *p_bkgcolor = 36;
    }
    else
    {
        ;
    }
}

void animate_EPU_TRQ_WARRING(float * p_data ){
    if (epu_trq > 100.0) {
        *p_data = 10.0-( epu_trq - 100.0);
    }
    else
    {
        *p_data = 10.0;
    }
}

/* Animation functions for plug EPU_T45 */
float _STEP_EPU_T45=0.1;
float epu_t45=0.0;

void animate_EPU_T45(float * p_data ){
    *p_data += _STEP_EPU_T45;
    if (*p_data >= 10.0) {
        *p_data = (float)10.0;
        _STEP_EPU_T45 = -_STEP_EPU_T45;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_EPU_T45 = -_STEP_EPU_T45;
    }
    epu_t45 = *p_data;
}

void animate_EPU_T45_COLOR(int * p_data,int * p_fontcolor,int * p_bkgcolor ){
    if (epu_t45 < 8.49) {
        *p_data = 11;
        *p_fontcolor = 11;
        *p_bkgcolor = 1;
    }
    else if (epu_t45 < 9.15) {
        *p_data = 46;
        *p_fontcolor = 1;
        *p_bkgcolor = 46;
    }
    else if (epu_t45 < 10.0) {
        *p_data = 36;
        *p_fontcolor = 60;
        *p_bkgcolor = 36;
    }
    else
    {
        ;
    }
}

void animate_EPU_T45_WARRING(float * p_data ){
    if (epu_t45 > 9.15) {
        *p_data = 8.45 - (epu_t45 - 9.15)*10.0;
    }
    else
    {
        *p_data = 8.45;
    }
}

/* Animation functions for plug FUEL_LEFT */
float _STEP_FUEL_LEFT=10;
float fuel_left=470.0;

void animate_FUEL_LEFT(float * p_data ){
    *p_data += _STEP_FUEL_LEFT;
    if (*p_data >= 470.0) {
        *p_data = (float)470.0;
        _STEP_FUEL_LEFT = -_STEP_FUEL_LEFT;
    } else if (*p_data <= 0.0) {
        *p_data = (float)0.0;
        _STEP_FUEL_LEFT = -_STEP_FUEL_LEFT;
    }
    fuel_left = *p_data;
}

void animate_FUEL_LEFT_COLOR(int * p_data,int * p_fontcolor,int * p_bkgcolor ){
    if (fuel_left < 47.0) {
        *p_data = 36;
        *p_fontcolor = 60;
        *p_bkgcolor = 36;
    }
    else
    {
        *p_data = 11;
        *p_fontcolor = 11;
        *p_bkgcolor = 1;
    }
}
