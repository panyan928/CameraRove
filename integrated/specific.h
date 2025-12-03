/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : specific.h
 DESCRIPTION       : Specific interfaces of the application
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

#ifndef _SPECIFIC_H
#define _SPECIFIC_H


#ifdef __cplusplus
extern "C" {
#endif

/* Interface to specific part */


    extern int getW();
    extern int getH();
    extern float getRatioX();
    extern float getRatioY();
    extern const char *getSpecName();

    extern void *getColorTable();
    extern int getColorTableSize();
    extern void *getLineWidthTable();
    extern int getLineWidthTableSize();
    extern void *getLineStippleTable();
    extern int getLineStippleTableSize();
    extern void *getFontTable();
    extern int getTextureTableSize();


    extern void context_init();
    extern void draw_init();
    extern void draw();

    extern void anim_init();
    extern void animate();

    extern void init_scene();
    extern void draw_scene();

    extern void oglx_init();
    extern void oglx_reset();
    extern void switch_line_mode();
    extern void DisplayOGLXErrors();

    extern void parseScade();

#ifdef __cplusplus
}
#endif
#endif                          /* _SPECIFIC_H */
