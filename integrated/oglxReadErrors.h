/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxReadErrors.h
 DESCRIPTION       : oglxReadErrors shall provide a readable strucure for error storage
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- +*/

#ifndef _OGLXREADERRORS_H
#define _OGLXREADERRORS_H

#include "sgl.h"

typedef struct {
    char *s_error_name;
    char *s_rationale;
    char *s_parameter_name;
    SGLulong ul_parameter_value;
} oglx_error;

typedef struct {
    char *s_error_name;
    char *s_rationale;
    char *s_parameter_name;
} oglx_error_definition;


OGLXAPI oglx_error *oglxReadErrors(SGLulong * par_ul_error_number);

OGLXAPI oglx_error_definition oglxGetErrorDefinition(SGLulong par_ul_error_index);

#endif                          /* _OGLXREADERRORS_H */
