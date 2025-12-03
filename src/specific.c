/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : specific.c
 DESCRIPTION       : Specific interfaces of the application
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

#include "oglx.h"
//#include "gl/gl.h"
#include <string.h>

#define ANIMATION 1
#if defined(COEXECUTION)
extern void Reset();
extern void Step();
#elif defined(ANIMATION)
#include "animation.h"
#endif
#include "specific.h"
/* Generated .H files for resource tables and specification properties */
#include "target_configuration.h"
#include "aol_color_table.h"
#include "aol_line_width_table.h"
#include "aol_line_stipple_table.h"
#include "aol_font_table.h"
#include "aol_texture_table.h"

/* TODO GENERATED INCLUDES: include in this section generated .H and .P files for each layer */
#include "aol_symbology_layer.h"
/* END GENERATED INCLUDES */
#include "oglxReadErrors.h"

static GLint timer_value = 20;  /* integer: Time (in ms) between two frames */
static GLint fps = 0;           /* integer: Number of displayed Frame Per Second */
static GLint pause = 0;         /* boolean: Enable/disable refresh of drawings */
static GLint rawlinemode = 0;   /* boolean: Enable/disable RAW OpenGL mode for OGLX lines */
static GLenum glErrorStatus;    /* enum: OpenGL error index */

static sgl_type_statemachine glob_s_context;

static aol_typ_symbology_layer NDcontext;



int getW()
{
    return target_screen_width;
}

int getH()
{
    return target_screen_height;
}

float getRatioX()
{
    return (float) ratio_x;
}

float getRatioY()
{
    return (float) ratio_y;
}

const char *getSpecName()
{
    return "PrimaryFlightDisplay";
}

void *getColorTable()
{
    return (void *) aol_color_table;
}

int getColorTableSize()
{
    return aol_color_table_size;
}

void *getLineWidthTable()
{
    return (void *) aol_line_width_table;
}

int getLineWidthTableSize()
{
    return aol_line_width_table_size;
}

void *getLineStippleTable()
{
    return (void *) (&aol_line_stipple_table);
}

int getLineStippleTableSize()
{
    return aol_line_stipple_table_size;
}

void *getFontTable()
{
    return (void *) (&aol_font_table);
}

int getTextureTableSize()
{
    return AOL_TEXTURE_TABLE_SIZE;
}

void context_init()
{
    aol_symbology_layer_init(&NDcontext);
}

void draw_init()
{
   aol_texture_table();
   //aol_static_sequences();
}


void draw()
{
    /* TODO DRAW LAYERS: call the draw function for each included layer */

    aol_symbology_layer_predraw(&NDcontext);
    aol_symbology_layer_draw(&NDcontext, 0);
    /* END DRAW LAYERS */
}

#if defined(ANIMATION)

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           anim_init
    DESCRIPTION:    Initialize the animations for each included layer
---------------------------------------------------------------------+*/

void anim_init()
{
    /* TODO INIT ANIMATION: add in this section the manual initialization code for plugs */

    aol_symbology_layer_S_ND_VIS(&NDcontext, SGL_TRUE);

    /* END INIT ANIMATION */

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           animate
    DESCRIPTION:    Computes the animations for each included layer
---------------------------------------------------------------------+*/

void animate()
{
    /* TODO RUN ANIMATION: add in this section the manual computation code for plugs */

    /* END RUN ANIMATION */
}

#endif

void init_scene()
{

/* If SCADE Suite COEXECUTION is enabled, plug values are initialized by SCADE Suite */
#if defined(COEXECUTION)
    Reset();

/* Else if ANIMATION only is enabled, plug values are initialized via hand-coded init functions */
#elif defined(ANIMATION)
    context_init();
    anim_init();
    draw_init();

/* Else plug values are default ones */
#else
    context_init();
    draw_init();

#endif
}

void draw_scene()
{

/* If SCADE Suite COEXECUTION is enabled, plug values are computed by SCADE Suite */
#if defined(COEXECUTION)
    Step();

/* Else if ANIMATION only is enabled, plug values are computed via hand-coded animation functions */
#elif defined(ANIMATION)
    oglx_reset();
    animate();
    draw();
    glDisable(GL_TEXTURE_2D);
/* Else plug values remain default ones (no moves) */
#else
    draw();

#endif
}

void oglx_init()
{
    static SGLbyte glob_tub_texture_buffer[4 * SGL_TEXTURE_MAX_WIDTH * SGL_TEXTURE_MAX_HEIGHT];
    sgl_texture_attrib *glob_texture_attrib = malloc(sizeof(sgl_texture_attrib) * getTextureTableSize());
    static sgl_parameters lParameters;

    lParameters.ul_screen_width = getW();
    lParameters.ul_screen_height = getH();
    lParameters.pb_texture_buffer = glob_tub_texture_buffer;
    lParameters.ul_texture_max_width = SGL_TEXTURE_MAX_WIDTH;
    lParameters.ul_texture_max_height = SGL_TEXTURE_MAX_HEIGHT;
    lParameters.p_texture_attrib = glob_texture_attrib;
    lParameters.ul_number_of_textures = getTextureTableSize();

    sglInit(&glob_s_context, &lParameters);

    sglColorPointerf(getColorTable(), getColorTableSize());

    /* Set the OGLX line mode (RAW/SMOOTH) and load the corresponding line width table */
    if (rawlinemode) {
        sglSetRenderMode(SGL_RAW_OPENGL_LINES);
    }
    else {
        sglSetRenderMode(SGL_SMOOTH_LINES);
    }

    sglLineWidthPointerf(getLineWidthTable(), getLineWidthTableSize());

    /* Load the line stipple table */
    sglLineStipplePointer(getLineStippleTable(), getLineStippleTableSize());

    /* Load the fonts table */
    sgluLoadFonts(getFontTable());

    //glShadeModel(GL_FLAT);

    sglViewport(0L, 0L, getW(), getH());
    sglOrtho(0, (float) (getW() * getRatioX() / 1.0F), 0, (float) (getH() * getRatioY() / 1.0F));

    /* Check if there is an OpenGL error after OGLX initialization */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during OGLX initialization\n\n", glErrorStatus);
    }
}

void oglx_reset()
{
    sglReset();
    sglViewport(0L, 0L, getW(), getH());
    sglOrtho(0, (float) (getW() * getRatioX() / 1.0F), 0, (float) (getH() * getRatioY() / 1.0F));

    /* Check if there is an OpenGL error after OGLX reset */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during OGLX reset\n\n", glErrorStatus);
    }

}

void switch_line_mode()
{
    rawlinemode = !rawlinemode;
    sglTerminate();
    oglx_init();
    draw_init();
}

void DisplayOGLXErrors()
{
    static SGLuint32 loc_errors_number = 0;
    SGLuint32 loc_last_errors_number = 0;
    SGLuint32 loc_errors[SGL_ERROR_MAX][2];

    /*  OGLX interface sglGetErrors fills the input table with the 32 first errors since the start of the application or since last call to sgl ClearErrors */
    SGLbyte loc_b_status = sglGetErrors((SGLulong *) loc_errors, &loc_last_errors_number);

    if (loc_last_errors_number != loc_errors_number) {
        loc_errors_number = loc_last_errors_number;

        /* Fill the last error index only if there is at least one error */
        if (loc_b_status != SGL_NO_ERROR) {
            SGLulong i = 0;

            printf("**************************************************************\n");
            printf("\n\nNumber of OGLX stored errors: %d\n", (int) loc_errors_number);
            for (i = 0; i < loc_errors_number; i++) {
                /* Utilitary function oglxGetErrorDefinition provides additional informations on the detected errors */
                oglx_error_definition loc_error = oglxGetErrorDefinition(loc_errors[i][0]);
                printf("\nError number %d\n", (int) i + 1);
                /* Print outputs provided by sglGetErrors */
                printf("Error ID: %d, Parameter value: %d\n", (int) loc_errors[i][0], (int) loc_errors[i][1]);
                /* Print outputs provided by oglxReadErrors */
                printf("Error Name: %s\nRationale: %s\nParameter name: %s\nParameter value: %d\n",
                       loc_error.s_error_name, loc_error.s_rationale, loc_error.s_parameter_name, (int) loc_errors[i][1]);
            }
        }
        else {
            printf("**************************************************************\n");
            printf("\n\nNo OGLX error.\n");
        }
    }
}

